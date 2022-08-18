


namespace my_stl
{
namespace stl_test
{
    // 比较两个容器
    template <class Tp, class Up, class Size>
    void __expect_con_eq(Tp container1, Up container2, Size n)
    {
        auto first1 = container1.begin();
        auto first2 = container2.begin();
        for( ; n > 0; --n, ++first1, ++first2)
        {
            EXPECT_EQ(*first1, *first2);
        }
    }
    // 比较原生数组
    template <class Tp, class Up, class Size>
    void __expect_con_eq(Tp* a, Up* b, Size n)
    {
        for(Size i = 0; i < n; ++i)
        {
            EXPECT_EQ(a[i], b[i]);
        }
    }
    template <class T, class U, class Size>
    void EXPECT_CON_EQ(T a, U b, Size n)
    {
        __expect_con_eq(a, b, n);
    }
}


}