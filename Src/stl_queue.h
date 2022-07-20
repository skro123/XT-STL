
#ifndef __SGI_STL_INTERNAL_QUEUE_H
#define __SGI_STL_INTERNAL_QUEUE_H

namespace my_stl 
{
    template <typename, typename> class queue;
    template <class T, class Sequence>
    bool operator== (const queue<T, Sequence>&, const queue<T, Sequence>&);
    template <class T, class Sequence>
    bool operator< (const queue<T, Sequence>&, const queue<T, Sequence>&);

    template <class T, class Sequence = deque<T> >
    class queue {
        friend bool operator== __STL_NULL_TMPL_ARGS (const queue& x, const queue& y);
        friend bool operator< __STL_NULL_TMPL_ARGS (const queue& x, const queue& y);

        public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef const reference const_reference;

        protected:
        Sequence c;

        public:
        bool empty() { return c.empty(); }
        size_type size() { return c.size(); }
        reference front() { return c.front(); }
        const_reference front() const { return c.front(); }
        reference back() { return c.back(); }
        const_reference back() const { return c.back();}
        void pop() { c.pop_front();}
        void push(const value_type& x) { c.push_back(x);}
    };
    template <class T, class Sequence>
    bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
    {
        return x.c == y.c;
    }
    template <class T, class Sequence>
    bool operator< (const queue<T, Sequence>& x, const queue<T, Sequence>& y)
    {
        return x.c < y.c;
    }


    template <class T, class Sequence =std::vector<T>, 
        class Compare = std::less<typename Sequence::value_type> >
    class priority_queue
    {
        public:
        // 内嵌类型声明
          typedef typename Sequence::value_type value_type;
          typedef typename Sequence::size_type size_type;
          typedef typename Sequence::reference reference;
          typedef typename Sequence::const_reference const_reference;

        protected:
          Sequence c;
          Compare comp;

        public:
          priority_queue() : c() {}
          explicit priority_queue(const Compare& x) : c(), comp(x) {}
          
          template <class InputIterator>
          priority_queue(InputIterator first, InputIterator last, const Compare& x)
            : c(first, last), comp(x) { make_heap(c.begin(), c.end(), comp); }
          template <class InputIterator>
          priority_queue(InputIterator first, InputIterator last)
            : c(first, last) { make_heap(c.begin(), c.end(), comp); }

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        const_reference top() const { return c.front(); }

        void push(const value_type& x)
        {
            try
            {
                c.push_back(x);
                push_heap(c.begin(), c.end(), comp);
            }
            catch(...)
            {
                c.clear();
            }
        }
        
        void pop()
        {
            try
            {
                pop_heap(c.begin(), c.end(), comp);
                c.pop_back();
            }
            catch(...)
            {
                c.clear();
            }
        }
    };
}

#endif /* __SGI_STL_INTERNAL_QUEUE_H */