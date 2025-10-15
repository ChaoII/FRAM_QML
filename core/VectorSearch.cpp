//
// Created by aichao on 2025/10/13.
//

#include "core/VectorSearch.h"
#include <iostream>
#include <faiss/index_factory.h>
#include <faiss/IndexIDMap.h>
#include <faiss/index_io.h>


/** 添加向量（带ID） */
void VectorSearch::addVector(const int64_t id, const std::vector<float>& vec) {
    if (index_ == nullptr) return;
    if (id_to_vec_dim_.count(id)) {
        std::cerr << "[VectorSearch] ID already exists, use updateVector() instead.\n";
        return;
    }
    index_->add_with_ids(1, vec.data(), &id);
    id_to_vec_dim_[id] = vec.size();
    dirty_ = true;
}

/** 批量添加 */
void VectorSearch::addVectors(const std::vector<int64_t>& ids,
                              const std::vector<std::vector<float>>& vecs) {
    if (vecs.empty()) return;
    size_t d = vecs[0].size();
    std::vector<float> flat;
    flat.reserve(vecs.size() * d);
    for (const auto& v : vecs) {
        flat.insert(flat.end(), v.begin(), v.end());
    }
    index_->add_with_ids(static_cast<int64_t>(vecs.size()), flat.data(), ids.data());
    for (auto& id : ids)
        id_to_vec_dim_[id] = d;
    dirty_ = true;
}

/** 查询（返回最相似的前k个ID） */
[[nodiscard]] std::vector<int64_t> VectorSearch::search(const std::vector<float>& query, const int k) const {
    if (index_ == nullptr) return {};
    std::vector<int64_t> indices(k);
    std::vector<float> distances(k);
    index_->search(1, query.data(), k, distances.data(), indices.data());
    return indices;
}

/** 删除某个向量 */
void VectorSearch::removeVector(const int64_t id) {
    if (index_ == nullptr) return;
    const faiss::IDSelectorRange selector(id, id + 1);
    index_->remove_ids(selector);
    id_to_vec_dim_.erase(id);
    dirty_ = true;
}

/** 更新向量（本质是删除后再添加） */
void VectorSearch::updateVector(const int64_t id, const std::vector<float>& vec) {
    removeVector(id);
    addVector(id, vec);
}

/** 查询是否存在某ID */
bool VectorSearch::hasVector(const int64_t id) const {
    return id_to_vec_dim_.count(id) > 0;
}

/** 获取向量总数 */
size_t VectorSearch::size() const {
    return index_ ? index_->ntotal : 0;
}

/** 手动保存索引 */
void VectorSearch::save(const std::string& path) {
    if (index_ && dirty_) {
        faiss::write_index(index_, path.c_str());
        dirty_ = false;
    }
}

VectorSearch::~VectorSearch() {
    // 程序退出自动保存
    save();
    delete index_;
}


VectorSearch::VectorSearch() {
    const std::string index_path = "index.faiss";
    const int d = 1024; // 默认维度，可按需修改

    if (std::filesystem::exists(index_path)) {
        try {
            index_ = faiss::read_index(index_path.c_str());
            std::cout << "[VectorSearch] Loaded existing index from " << index_path << std::endl;
        }
        catch (...) {
            std::cerr << "[VectorSearch] Failed to load index, creating new one.\n";
            index_ = createIndex(d);
        }
    }
    else {
        index_ = createIndex(d);
    }
    dirty_ = false;
}

faiss::IndexIDMap* VectorSearch::createIndex(const int d) {
    auto* base = faiss::index_factory(d, "Flat", faiss::METRIC_INNER_PRODUCT);
    auto* idmap = new faiss::IndexIDMap(base);
    std::cout << "[VectorSearch] Created new index (dim=" << d << ")\n";
    return idmap;
}
