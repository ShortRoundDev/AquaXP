#pragma once

#include <set>
#include "AABB.h"

namespace AquaXP
{

    constexpr sz MAX_OCTREE_BUCKET_SIZE = 8;

    template<typename T, template<typename> typename Alloc = std::allocator>
    class OctreeNode
    {
        using AllocType = Alloc<OctreeNode>;
        using AllocTraits = std::allocator_traits<AllocType>;
        static_assert(std::is_same_v<typename AllocTraits::value_type, OctreeNode>,
            "Allocator must be for OctreeNode type");
    public:
        OctreeNode(Alloc<OctreeNode> const& allocator = Alloc<OctreeNode>()) :
            m_allocator(allocator),
            m_bounds(AABB(DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 0, 0, 0))),
            m_isLeaf(true),
            m_children(nullptr) { }

        OctreeNode(AABB const& bounds, Alloc<OctreeNode> const& allocator = Alloc<OctreeNode>()) :
            m_allocator(allocator),
            m_bounds(bounds),
            m_isLeaf(true),
            m_children(nullptr) { }

        bool tryInsert(std::pair<AABB, T*> value)
        {
            AABB const& container = std::get<0>(value);
            T* data = std::get<1>(value);
            if (!m_bounds.overlap(container))
            {
                return false;
            }
            m_values.push_back(value);
            if (!m_isLeaf)
            {
                for (int i = 0; i < 8; i++)
                {
                    m_children[i].tryInsert(value);
                }
            }
            else if (m_values.size() > MAX_OCTREE_BUCKET_SIZE)
            {
                subdivide();
            }
            return true;
        }

        template<typename Comp = std::less<T*>>
        sz tryQuery(AABB const& query, std::set<T*, Comp, Alloc<T*>>& results)
        {
            if (!query.overlap(m_bounds))
            {
                return 0L;
            }
            sz count = 0;
            if (m_isLeaf)
            {
                for (auto value : m_values)
                {
                    auto const& aabb = std::get<0>(value);
                    if (aabb.overlap(m_bounds))
                    {
                        results.emplace(std::get<1>(value));
                        count++;
                    }
                }
                return count;
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    count += m_children[i].tryQuery(query, results);
                }
                return count;
            }
        }

        sz tryQuery(AABB const& query, T** results, sz maxSize, sz iterator = 0)
        {
            if (!query.overlap(m_bounds) || iterator >= maxSize)
            {
                return 0L;
            }
            sz count = 0;
            if (m_isLeaf)
            {
                for (auto value : m_values)
                {
                    auto const& aabb = std::get<0>(value);
                    if (aabb.overlap(m_bounds))
                    {
                        results[iterator++] = std::get<1>(value);
                        count++;
                        if (iterator >= maxSize)
                        {
                            return count;
                        }
                    }
                }

                return count;
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    auto currentCount = count;
                    count += m_children[i].tryQuery(query, results, iterator);
                    iterator += (count - currentCount);
                    if (iterator >= maxSize)
                    {
                        return count;
                    }
                }

                return count;
            }

        }

    private:
        Alloc<OctreeNode> m_allocator;
        bool m_isLeaf;
        AABB m_bounds;
        std::shared_ptr<OctreeNode[]> m_children;
        std::vector<std::pair<AABB, T*>, Alloc<std::pair<AABB, T*>>> m_values;

        void subdivide()
        {
            m_isLeaf = false;
            Alloc<OctreeNode> allocator = Alloc<OctreeNode>();
            m_children = std::allocate_shared_for_overwrite<OctreeNode[], Alloc<OctreeNode>>(allocator, 8);
            auto defVal = AABB(DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 0, 0, 0));
            std::array<AABB, 8> childContainers { defVal, defVal, defVal, defVal, defVal, defVal, defVal, defVal };
            SubdivideAABB(m_bounds, childContainers);
            for (i32 i = 0; i < 8; i++)
            {
                m_children[i] = OctreeNode<T, Alloc>(childContainers[i], m_allocator);
                for (auto& value : m_values)
                {
                    m_children[i].tryInsert(value);
                }
            }
        }
    };
}