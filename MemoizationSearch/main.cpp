#include <iostream>
#include"MemoizationSearch.h"

#include <stddef.h>

#include <string>
#ifdef _WIN32
#include<Windows.h>
#include <Psapi.h>
#endif // _WIN32
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <locale.h>
#include <codecvt>
#include<ProcessSnapshot.h>
int foo1() {
    std::cout << "foo1"<<" ";
    return 36;
}
#ifdef _WIN32
#define INT64  unsigned __int64
#else
#define INT64 unsigned long long
#endif
inline  INT64  Fibonacci(int n);
auto fib = MakeCache(Fibonacci);//生成一个斐波那契的缓存函数
inline  INT64  Fibonacci(int n) {
   if (n <= 1) return n;
   return fib(n - 1) + fib(n - 2);
}
static inline std::string WideStringToString(const std::wstring& wideString) {
    std::cout << "WideStringToString called!"<<std::endl;
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wideString);
}
auto WideStringToStringCached = MakeCache(WideStringToString);//0xFFFF'FFFF'FFFF'FFFF代表有效期无 永远也不会过
#include <chrono>
#include <cmath>
#include <vector>

template<class PRE>
INT64 TestSpeed(const PRE& pre, INT64 initcount = 1e+3) noexcept {
    INT64 int_count = initcount; // 初始测试次数
    const int num_runs = 20; // 每次运行的次
    std::vector<double> results(num_runs); // 存储每次运行的结
    const double max_total_time = 5.0; // 最大总运行时间（秒）
    const double target_time_per_run = 0.1; // 每次运行的目标时间（秒）

    while (true) {
        double total_time = 0.0;
        double total_result = 0.0;
        for (int run = 0; run < num_runs; ++run) {
            auto start = std::chrono::high_resolution_clock::now();
            for (INT64 i = 0; i < int_count; ++i) pre();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            results[run] = int_count / elapsed.count(); // 每秒执行次数
            total_result += results[run];
            total_time += elapsed.count();
        }
        double avg_result = total_result / num_runs;

        // 计算标准
        double sum_sq_diff = 0.0;
        for (const auto& res : results) {
            double diff = res - avg_result;
            sum_sq_diff += diff * diff;
        }
        double stddev = std::sqrt(sum_sq_diff / num_runs);

        // 判断是否达到稳定性（相对标准差小 1%
        if ((stddev / avg_result) < 0.1 || total_time > max_total_time) {
            return static_cast<INT64>(avg_result);
        }

        // 根据运行时间调整 int_count
        if (total_time < target_time_per_run * num_runs) {
            int_count *= 2; // 增加测试次数
        }
        else {
            int_count += int_count / 2; // 增加 50%
        }

        // 防止测试次数过大
        if (int_count > 1e+20) break;
    }
    return 0;
}

std::string formatWithCommas(INT64 value) {
    std::string num = std::to_string(value);
    int insertPosition = (int)num.length() - 3;
    while (insertPosition > 0) {
        num.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return num;
}

// 使用一个矩阵来存储每月的最低成本，每列表示一个库存水

#undef max
#undef min
int  add() {
    return 0;
}
struct calltest {
    int add(int x,int y) {
        return x+y;
    }
    int print() {
        return 35;
    }
};
//写一个大运算的类
struct BigNumber {
};
auto addcache = MakeCacheEx(add, memoizationsearch::nonstd::CallType::cdeclcall, 200);
class RandomWrapper {
public:
    RandomWrapper() : count(0) {
        std::random_device rd;
        gen = std::mt19937(rd());
        dis = std::uniform_int_distribution<>(1, 100);
        current_value = dis(gen);
    }

    int get_random_value() {
        count++;
        if (count % 20 == 0) {
            current_value = dis(gen); // 10次重新生成随机
        }
        return current_value;
    }

private:
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    int current_value;
    int count;
};
template <typename T> struct DebugClass {
    T data; // This is a member of type T
    DebugClass(const T& obj) : data(obj) {
        std::cout << typeid(T).name() << " constructor called" << std::endl;
    }
    operator T() {
        return data;
    }
    ~DebugClass() {
        std::cout << typeid(T).name() <<" destruct for " << this << std::endl;
    }
    DebugClass(const DebugClass& other) {
        std::cout << typeid(T).name() << " copy constructor called" << std::endl;
    }
    DebugClass(DebugClass&& other) {
        std::cout << typeid(T).name() << " move constructor called" << std::endl;
    }
    DebugClass& operator=(const DebugClass& other) {
        std::cout << typeid(T).name() << " copy assignment operator called" << std::endl;
        return *this;
    }
    DebugClass& operator=(DebugClass&& other) {
        std::cout << typeid(T).name() << " move assignment operator called" << std::endl;
        return *this;
    }
    T* operator->() {
        return &data;
    }
};
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <functional>
#include <unordered_set>
template<typename... Args>
class Cache {
    using KeyType = std::tuple<Args...>;
    std::unordered_map<KeyType, std::string> cache;
    mutable typename std::unordered_map<KeyType, std::string>::iterator it = cache.end();

public:
    std::string& getValue(const Args&... args) {
        KeyType key = std::make_tuple(args...);
        if (it != cache.cend() && it->first == key) {
            // 使用静态迭代器的值，如果它有效且匹配
            return it->second;
        }

        it = cache.find(key);
        if (it != cache.end()) {
            return it->second;  // 找到有效的缓存项
        }
        else {
            // 模拟数据库查询或其他耗时操作
            std::string value = "New Data";
            cache[key] = value;
            it = cache.find(key);  // 更新迭代
            return it->second;
        }
    }
};
int sub(int a, int b) {
    DebugClass < std::string> dbgobj("66");
    std::ignore=dbgobj->c_str();
    return 0;
}
template<typename T>
T Read(LPVOID addr) {
    SIZE_T nRet = 0;
	T tRead{};
    ReadProcessMemory(GetCurrentProcess(), addr, &tRead, sizeof(T), &nRet);
    return tRead;
}
template <typename Tuple, std::size_t... I>
bool compare_impl(Tuple&& t1, Tuple&& t2, std::index_sequence<I...>) {
    std::cout << "compare_impl" << std::endl;
    bool result = true;
    using expander = int[];
    (void)expander {
        0, (void(result = result && std::get<I>(std::forward<Tuple>(t1)) == std::get<I>(std::forward<Tuple>(t2))), 0)...
    };
    return result;
}


template <typename... Args>
bool compare_tuples(std::tuple<Args...>&& t1, std::tuple<Args...>&& t2) {
    return compare_impl(std::forward<std::tuple<Args...>>(t1), std::forward<std::tuple<Args...>>(t2), std::make_index_sequence<sizeof...(Args)>());
}
class SimpleMutex {
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    SimpleMutex() {
        lock();
    }
    ~SimpleMutex() {
        unlock();
    }
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) {
            // Busy-wait
            std::this_thread::yield();
        }
    }

    void unlock() {
        locked.clear(std::memory_order_release);
    }
};
void pirnthello() {
    SimpleMutex mutex;
    std::cout << "hello world" << std::endl;
}
static  auto program_start = std::chrono::system_clock::now().time_since_epoch();//程序开始的时间的时间戳
static inline unsigned long  approximategetcurrenttime() noexcept {
    //AUTOLOG //自动记录日志
    return std::chrono::duration_cast<std::chrono::microseconds>(program_start).count();//直接返回当前时间 program_start是程序开始的时间

}

bool CheckPtr(uintptr_t addr){
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((LPVOID)addr, &mbi, sizeof(mbi)) == 0) {
        return false;
    }
    if (mbi.Protect & PAGE_NOACCESS) {
        return false;
    }
    mbi.Protect &= ~PAGE_GUARD;
    mbi.Protect &= ~PAGE_NOCACHE;
    return mbi.Protect & PAGE_READONLY || mbi.Protect & PAGE_READWRITE || mbi.Protect & PAGE_WRITECOPY;
}
auto CheckPtrCache = MakeCache(CheckPtr);
int main() {
    auto cachedlambda = MakeCache([](int a, int b) {
        std::cout << "foo" << "\n";
        return a + b;
    });
    auto cachelambdafirst = MakeCache([](int a) {
        return a;
    });
    //无参数的函数的缓存版
    auto noparam = MakeCache(foo1);
    auto noparam2 = MakeCache(foo1);
    std::cout<<"noparam ptr:" << noparam.Raw()<<std::endl;//裸指针调 只会创建一
    std::cout<<"noparam2 ptr:" << noparam2.Raw()<<std::endl;//裸指针调
    std::cout << cachedlambda(35, 99) << "\n";//有参数的情况

    std::cout << cachedlambda(35, 99) << "\n";//有参数的情况
    cachedlambda.cleancache(std::make_tuple(35, 99));//清除缓存
    std::cout << cachedlambda(35, 64) << "\n";//有参数的情况
    //无参数函数的缓存版本
    std::cout << noparam() << "\n";
    std::cout << noparam() << "\n";
    noparam.cleancache();//清除所有缓
    std::cout << noparam() << "\n";
    //无参数lambda的缓存版
    //输出函数类型
    std::cout << noparam.GetObjectName() << "\n";
    memoizationsearch::nonstd::CachedFunction<std::string> handmadevoidfunc{ std::make_pair("msg box", INFINITYCACHE) };
    std::cout <<"handmadevoidfunc:" << handmadevoidfunc() << "\n";

    //std::cout << fib(256) << "\n";
    std::ofstream outfile("fibFibonacci", std::ios::binary|std::ios::trunc);
    fib >> outfile;//流式写入 从内存到文件
    //从文件读取缓
    WideStringToStringCached.loadcache("WideStringToStringCached");
    std::cout << WideStringToStringCached(L"what's happen?").c_str() << std::endl;
    std::cout << WideStringToStringCached(L"hello world").c_str() << std::endl;
    std::cout<<"narro:"<<WideStringToStringCached.raw()<<std::endl;
    //对象是可以移动的
    auto test = std::move(WideStringToStringCached);
    //对象是可以复制的
    auto test2 = test;
    for (auto& pair : test2) {//支持遍历
        std::cout << pair.second.first.c_str() << std::endl;
    }
    if (test)std::cout << "test is callable" << std::endl;
    //重新转换为function
    std::function< std::string(const std::wstring&)> func = test;
    auto newcache = MakeCache(WideStringToString);//新创建的缓存函数里面没有东西
    std::cout <<"CacheTime:" << newcache.getcachetime()<<"ms" << std::endl;
    newcache.setcachetime(580);
    newcache(L"WIDETONARRO");//newcache可以被调用WIDETONARRO是没被缓存过
    //把newcache的缓存内容弄到test里面
    Sleep(10);
    test << newcache;
    std::cout << "print------------------" << std::endl;
    //支持两种方式遍历
    for (const auto& pair : test) {//支持遍历
        std::cout << pair.second.first.c_str() << std::endl;
    }
    for (auto iter = test.begin(); iter != test.end(); ++iter) {
        std::cout << (*iter).second.first.c_str() << std::endl;
    }
    std::cout << "print----------------------" << std::endl;
    //输出缓存的个
    std::cout << "count:" << test << std::endl;
    test.savecache("WideStringToStringCached");
    std::cout << "others----------------------" << std::endl;
#ifdef _WIN64
    auto K32GetPerformanceInfoCaChe = MakeCacheEx(K32GetPerformanceInfo, memoizationsearch::nonstd::CallType::stdcall, (TimeType)MEMOIZATIONSEARCH);//stdcall的调用方,默认是cdecl
    PERFORMANCE_INFORMATION pi{};
    //调用缓存函数
    K32GetPerformanceInfoCaChe(&pi, sizeof(pi));
    //保存函数到文
    K32GetPerformanceInfoCaChe.savecache("K32GetPerformanceInfoCache");
    std::cout << "ProcessCount:" << pi.ProcessCount<< std::endl;
    //打印函数类型
    std::cout << typeid(decltype(K32GetPerformanceInfoCaChe.m_func)).name() << std::endl;
    //获取函数地址
    std::cout << &K32GetPerformanceInfoCaChe << std::endl;//转函数指
    auto funcptr=&K32GetPerformanceInfoCaChe;
    std::cout <<funcptr << std::endl;
    std::cout << typeid(decltype(funcptr)).name() << std::endl;
    std::cout << "ProcessCount Raw:" << pi.ProcessCount << std::endl;
    //检验函数是否可调用
    if (!K32GetPerformanceInfoCaChe)std::cout << "invalid function" << std::endl;
#endif
    calltest obj;
    auto cacheadd = CacheMemberFunction(obj, &calltest::add);//成员函数变为缓存函数
    std::cout<<cacheadd(3,5)<<"\n";
    //CacheMemberFunction原理
    auto memfn = std::mem_fn(&calltest::add);
    std::function<int(int, int)> func1 = [&obj, memfn](int x, int y) { return memfn(obj, x, y); };
    auto memcached = MakeCache(func1);
    std::cout<<memcached(6, 8)<<std::endl;
    auto cachedprint = CacheMemberFunction(obj, &calltest::print);
    std::cout << cachedprint() << std::endl;
    HCALLBACK callback = fib.addfiltercallbacks([](auto ret, auto pack) {
        return true;
    });
    std::cout << "-----------------" << std::endl;
    std::cout << "-----------------" << std::endl;
    auto speed = TestSpeed([]() {
        fib(2);
    });
    std::cout << speed << "/s" << std::endl; 
    std::cout << "-----------------" << std::endl;
    std::cout << fib(10) << std::endl;
    system("pause");
}