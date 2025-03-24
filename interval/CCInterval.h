#pragma once
#include <vector>
#include <tuple>
#include <optional>

namespace Algorithm
{
    template<class _Ty>
    struct IntervalNode
    {
        _Ty left;
        _Ty right;
    };

    template<class _Ty>
    class CCInterval
    {
    protected:
        std::vector<_Ty> m_segments;
    public:
        CCInterval() {}
        ~CCInterval() {}
        auto add_segment(IntervalNode<_Ty>& node)->bool
        {
            return add_segment(node.left, node.right);
        }

        auto add_segment(_Ty left, _Ty right)->bool
        {
            if (left > right) return false;

            // 按从小到大顺序放置
            int i;

            for (i = 0; i < m_segments.size() / 2; i++)
            {
                if (left < m_segments[2 * i] && right < m_segments[2 * i + 1])
                {
                    m_segments.insert(m_segments.begin() + 2 * i, left);
                    m_segments.insert(m_segments.begin() + 2 * i + 1, right);
                    break;
                }
            }

            if (i >= m_segments.size()/2)
            {
                m_segments.push_back(left);
                m_segments.push_back(right);
            }

            return true;
        }

        auto find_segment(_Ty number)->std::optional<const IntervalNode<_Ty>>
        {
            int l = 0;
            int h = m_segments.size() - 1;

            while (l <= h)
            {
                int m = (l + h) / 2;

                if (m & 1)
                {
                    if (number >= m_segments[m - 1] && number <= m_segments[m])
                    {
                        return IntervalNode<_Ty>
                        {
                            .left = m_segments[m - 1],
                            .right = m_segments[m]
                        };
                    }
                    
                    if (number < m_segments[m - 1]) h = m - 1;
                    else if (number > m_segments[m]) l = m + 1;
                }
                else
                {
                    if (number >= m_segments[m] && number <= m_segments[m + 1])
                    {
                        return IntervalNode<_Ty>
                        {
                            .left = m_segments[m],
                            .right = m_segments[m + 1]
                        };
                    }

                    if (number < m_segments[m]) h = m - 1;
                    else if (number > m_segments[m + 1]) l = m + 1;
                }
            }

            return std::nullopt;
        }

        auto find_all_overlaps()->std::vector<std::tuple<const IntervalNode<_Ty>, const IntervalNode<_Ty>>>
        {
            std::vector<std::tuple<const IntervalNode<_Ty>, const IntervalNode<_Ty>>> findOverlaps;

            for (int i = 0; i < m_segments.size() / 2 - 1; i++)
            {
                for (int j = i + 1; j < m_segments.size() / 2; j++)
                {
                    if (m_segments[2 * i] <= m_segments[2 * j] && m_segments[2 * i + 1] >= m_segments[2 * j])
                    {
                        std::tuple<const IntervalNode<_Ty>, const IntervalNode<_Ty>> overlaps
                        {
                            IntervalNode<_Ty>
                            {
                                .left = m_segments[2 * i],
                                .right = m_segments[2 * i + 1]
                            },
                            IntervalNode<_Ty>
                            {
                                .left = m_segments[2 * j],
                                .right = m_segments[2 * j + 1]
                            },
                        };

                        findOverlaps.push_back(overlaps);
                    }
                }
            }

            return findOverlaps;
        }
    };
};


