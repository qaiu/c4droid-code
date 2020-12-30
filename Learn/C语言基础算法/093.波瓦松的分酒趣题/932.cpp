//http://bbs.bccn.net/m.thread.php?tid=477181
//处理4个瓶子的分酒问题 请使用g++编译
#include <iostream>
#include <vector>
#include <queue>
#include <set>

bool bar( unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3
         , unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3
         , unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3 );

int main( void )
{
    bar( 12,0,0,0,  12,10,6,3,  4,4,4,0 );
    // 输出
    // 12      0       0       0
    // 2       10      0       0
    // 2       4       6       0
    // 2       1       6       3
    // 8       1       0       3
    // 11      1       0       0
    // 11      0       0       1
    // 1       10      0       1
    // 1       4       6       1
    // 1       4       4       3
    // 4       4       4       0

    bar( 12,0,0,0,  12,10,6,3,  3,3,3,3 );
    // 输出
    // 12      0       0       0
    // 6       0       6       0
    // 3       0       6       3
    // 3       3       6       0
    // 3       3       3       3

    bar( 12,0,0,0,  12,10,6,3,  5,3,2,2 );
    // 输出
    // 无法移动

    return 0;
}

struct foo
{
    unsigned char a[4]; // 当前容量
    static unsigned char b[4]; // 最大容量
    static unsigned char c[4]; // 最终容量

    foo( unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3 )
    {
        a[0]=a0, a[1]=a1, a[2]=a2, a[3]=a3;
    }

    bool operator<( const foo& other ) const
    {
        for( size_t i=0; i!=4; ++i )
        {
            if( a[i] < other.a[i] ) return true;
            if( a[i] > other.a[i] ) return false;
        }
        return false;
    }

    bool move( size_t from, size_t to )
    {
        if( from!=to && a[from]!=0 && a[to]!=b[to] )
        {
            unsigned char tmp = std::min( a[from], (unsigned char)(b[to]-a[to]) );
            a[from] -= tmp;
            a[to] += tmp;
            return true;
        }
        return false;
    }

    bool isok( void ) const
    {
        for( size_t i=0; i!=4; ++i )
            if( a[i] != c[i] ) return false;
        return true;
    }
};
unsigned char foo::b[4];
unsigned char foo::c[4];

std::ostream& operator<<( std::ostream& os, const foo& f )
{
    return os << (unsigned)f.a[0] << '\t' << (unsigned)f.a[1] << '\t' << (unsigned)f.a[2] << '\t' << (unsigned)f.a[3] << '\n';
}

bool bar( unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3
         , unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3
         , unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3 )
{
    foo::b[0]=b0, foo::b[1]=b1, foo::b[2]=b2, foo::b[3]=b3;
    foo::c[0]=c0, foo::c[1]=c1, foo::c[2]=c2, foo::c[3]=c3;

    std::queue<std::vector<foo>> steps;
    steps.push( std::vector<foo>(1,foo(a0,a1,a2,a3)) );

    if( steps.back().back().isok() )
    {
        std::cout << "不需要移动\n" << std::endl;
        return true;
    }

    std::set<foo> unique;
    unique.insert( steps.back().back() );

    while( !steps.empty() )
    {
        for( size_t i=0; i!=4; ++i )
        {
            for( size_t j=0; j!=4; ++j )
            {
                foo new_foo = steps.front().back();
                if( new_foo.move(i,j) ) // 可移
                {
                    if( unique.insert(new_foo).second ) // 这种状态之前未出现过
                    {
                        steps.push( steps.front() );
                        steps.back().push_back( new_foo );

                        if( new_foo.isok() ) // 符合要求
                        {
                            for( std::vector<foo>::const_iterator itor=steps.back().begin(); itor!=steps.back().end(); ++itor )
                                std::cout << *itor;
                            std::cout << std::endl;
                            return true;
                        }
                    }
                }
            }
        }

        steps.pop();
    }

    std::cout << "无法移动\n" << std::endl;
    return false;
}