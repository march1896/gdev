#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>

namespace Device {
    // TODO: move this to some utility header.
    // TODO: this does not work when U is V
    // TODO: is this still needed?
    template <typename U, typename V>
    class GenericBinding
    {
    protected:
        typedef std::pair<U, V> Entry;
        typedef std::vector<Entry> BindingList;
        BindingList m_bindings;

    public:
        inline void bind(U const& u, V const& v)
        {
            for (typename BindingList::iterator itr = m_bindings.begin(); itr != m_bindings.end(); itr ++)
            {
                if (itr->first == u)
                {
                    itr->second = v;
                    return;
                }
                else if (itr->second == v)
                {
                    itr->first = u;
                    return;
                }
            }

            m_bindings.push_back(std::make_pair(u, v));
        }

        inline U query(V const& v, bool& found) const
        {
            for (typename BindingList::const_iterator itr = m_bindings.begin(); itr != m_bindings.end(); itr ++)
            {
                if (itr->second == v)
                {
                    found = true;
                    return itr->first;
                }
            }
            found = false;
        }

        inline V query(U const& u, bool& found) const
        {
            for (typename BindingList::const_iterator itr = m_bindings.begin(); itr != m_bindings.end(); itr ++)
            {
                if (itr->first == u)
                {
                    found = true;
                    return itr->second;
                }
            }
            found = false;
        }
    };

}

#endif // _UTILS_H_
