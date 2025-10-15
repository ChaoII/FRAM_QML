//
// Created by aichao on 2025/10/13.
//

#pragma once
#include <faiss/IndexIDMap.h>
#include <unordered_map>
#include <filesystem>

class VectorSearch {
public:
    static VectorSearch& getInstance() {
        static VectorSearch instance;
        return instance;
    }

    /** 添加向量（带ID） */
    void addVector(int64_t id, const std::vector<float>& vec);

    /** 批量添加 */
    void addVectors(const std::vector<int64_t>& ids,
                    const std::vector<std::vector<float>>& vecs);

    /** 查询（返回最相似的前k个ID） */
    [[nodiscard]] std::pair<std::vector<int64_t>, std::vector<float>> search(
        const std::vector<float>& query, int k) const;

    /** 删除某个向量 */
    void removeVector(int64_t id);

    /** 更新向量（本质是删除后再添加） */
    void updateVector(int64_t id, const std::vector<float>& vec);
    /** 查询是否存在某ID */
    [[nodiscard]] bool hasVector(int64_t id) const;

    /** 获取向量总数 */
    [[nodiscard]] size_t size() const;

    /** 手动保存索引 */
    void save(const std::string& path = "index.faiss");

    ~VectorSearch();

    VectorSearch(const VectorSearch&) = delete;
    VectorSearch(VectorSearch&&) = delete;
    VectorSearch& operator=(const VectorSearch&) = delete;

private:
    VectorSearch();
    static faiss::IndexIDMap* createIndex(int d);

    faiss::Index* index_ = nullptr;
    // 用于记录哪些ID存在 本身不应该存在这个问题，因为后面会使用雪花算法生成ID，但是为了避免冲突，这里还是加上了
    std::unordered_map<int64_t, size_t> id_to_vec_dim_;
    bool dirty_ = false;
};
