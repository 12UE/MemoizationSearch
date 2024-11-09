/*
 * File name: MemorizationSearch.h Version:2.0.0
 * Copyright (C) 2024 12UE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Author: 12UE
 * Date: 2024-11-09
 * Location: Fuzhou, Fujian, China
 * Email: 1258432472@qq.com
 * Project：https://github.com/12UE/MemoizationSearch
 */
#ifndef  MEMOIZATIONSEARCH
#include <iostream>
#include <functional>
#include <unordered_map>
#include <chrono>
#include <type_traits>
#include <mutex>
#include <typeindex>
#include <random>
#include <future>
#include <fstream>
#include <initializer_list>
#include <type_traits>
#include <limits> 
#ifdef _DEBUG
#include <iostream>
#include <string>
#endif
#include <array>
#include <utility>
#include <cstdarg>
#include <algorithm>
#include <stack>
template<typename T>
static T getRandom(T min, T max) {
    static std::random_device rd;  // 用于获取随机种子
    static std::mt19937 gen(rd()); // Mersenne Twister 生成器
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dis(min, max);
        return dis(gen);
    }else {
        std::uniform_real_distribution<T> dis(min, max);
        return dis(gen);
    }
}
static constexpr size_t MAX_QUEUE_SIZE = 100; // 或其他适当的值 迭代的最大深度
using TimeType = unsigned long long;
using HCALLBACK = TimeType;//回调句柄 不用手动释放
#define INVALID_CALLBACK_HANDLE (HCALLBACK)-1//无效的回调句柄
#define ValidCallBackHandle(hCallBack) (bool)(hCallBack!= INVALID_CALLBACK_HANDLE)&&(hCallBack)
namespace xorstr_impl {
#ifdef _MSC_VER
#define XORSTR_INLINE __forceinline
#else
#define XORSTR_INLINE inline
#endif
    constexpr auto time = __TIME__;
    constexpr auto seed = static_cast<int>(time[7]) +
        static_cast<int>(time[6]) * 10 +static_cast<int>(time[4]) * 60 +static_cast<int>(time[3]) * 600 +static_cast<int>(time[1]) * 3600 +static_cast<int>(time[0]) * 36000;
    template <int N>
    struct random_generator {
    private:
        static constexpr unsigned a = 16807;  // 7^5
        static constexpr unsigned m = 2147483647;  // 2^31 - 1
        static constexpr unsigned s = random_generator<N - 1>::value;
        static constexpr unsigned lo = a * (s & 0xFFFF);  // multiply lower 16 bits by 16807
        static constexpr unsigned hi = a * (s >> 16);  // multiply higher 16 bits by 16807
        static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16);  // combine lower 15 bits of hi with lo's upper bits
        static constexpr unsigned hi2 = hi >> 15;  // discard lower 15 bits of hi
        static constexpr unsigned lo3 = lo2 + hi;
    public:
        static constexpr unsigned max = m;
        static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
    };
    template <>
    struct random_generator<0> {
        static constexpr unsigned value = seed;
    };
    template <int N, int M>
    struct random_int {
        static constexpr auto value = random_generator<N + 1>::value % M;
    };
    template <int N>
    struct random_char {
        static const char value = static_cast<char>(1 + random_int<N, 0x7F - 1>::value);
    };
    template <size_t N, int K>
    struct string {
    private:
        const char key_;
        std::array<char, N + 1> encrypted_;
        constexpr char enc(char c) const {
            return c ^ key_;
        }
        char dec(char c) const {
            return c ^ key_;
        }
    public:
        template <size_t... Is>
        constexpr XORSTR_INLINE string(const char* str, std::index_sequence<Is...>) :
            key_(random_char<K>::value), encrypted_{ { enc(str[Is])... } } {}
        XORSTR_INLINE decltype(auto) decrypt() {
            for (size_t i = 0; i < N; ++i) {
                encrypted_[i] = dec(encrypted_[i]);
            }
            encrypted_[N] = '\0';
            return encrypted_.data();
        }
    };
#undef XORSTR_INLINE
}  // namespace xorstr_impl
#define xorstr(s) (xorstr_impl::string<sizeof(s) - 1, \
  __COUNTER__>(s, std::make_index_sequence<sizeof(s) - 1>()).decrypt())
float  MEMOIZATIONSEARCH = 75.0f;//默认的缓存有效时间75ms
float& GetGlobalDefaultCacheTimeRef() {//获取全局的缓存时间的引用
    return   MEMOIZATIONSEARCH;
}
#ifdef _WIN64

 auto INFINITYCACHE = static_cast<TimeType>(0x0000'FFFF'FFFF'FFFF'FFFFULL);//定义缓存的最大时间
#else
using TimeType = unsigned long;
 auto INFINITYCACHE = static_cast<TimeType>(0xFFFF'FFFF);//定义缓存的最大时间在32位下
#endif // _WIN64
#if defined(__GNUC__) || defined(__clang__)
#define LIKELY(x)   __builtin_expect((x), 1)//GCC/Clang下的likely
#define UNLIKELY(x)   __builtin_expect((x), 0)//GCC/Clang下的unlikely
#else
#define LIKELY(x)   (x)//MSVC下没有这个东西
#define UNLIKELY(x) (x)//MSVC下没有这个东西
#endif
#ifdef _MSC_VER
#pragma warning( push )//保存当前的警告状态
#pragma warning(disable:4984)//禁止constexpr在C++14之前的版本的警告
#define SAFE_BUFFER __declspec(safebuffers)//在MSVC下表示函数的缓存区是安全的 但是在GCC下是没有这个东西的
#define CDECLCALL __cdecl//在MSVC下表示函数调用约定是cdecl 但是在GCC下是没有这个东西的
#ifdef _WIN64
#define STDCALL __stdcall//仅仅下64位的Windows下有这个东西MSVC64位下有以下东西
#define FASTCALL __fastcall//仅仅下64位的Windows下有这个东西MSVC64位下有以下东西
#define THISCALL __thiscall//仅仅下64位的Windows下有这个东西MSVC64位下有以下东西
#endif
#else
#define SAFE_BUFFER//在GCC/Clang下没有这个东西 定义为空
#define CDECLCALL//在GCC/Clang下没有这个东西 定义为空
#define STDCALL//在GCC/Clang下没有这个东西 定义为空
#define FASTCALL//在GCC/Clang下没有这个东西 定义为空
#define THISCALL//在GCC/Clang下没有这个东西 定义为空
#endif
#ifdef _DEBUG
 struct AutoLog {
     using Clock = std::chrono::high_resolution_clock;
     std::recursive_timed_mutex logmtx;
     Clock::time_point timepoint;
     std::string funcname;//存储函数名字
     SAFE_BUFFER inline AutoLog(const std::string& szName = "", const std::string& perfix = "") noexcept {
         timepoint = Clock::now();
         std::unique_lock<decltype(logmtx)> lock(logmtx);//加锁使得多线程下不会出现问题
         funcname = szName + " " + perfix;//函数的名字拼接上前缀
#ifdef DEBUG_LOG
         std::cout << funcname << xorstr("---> Begin\n");//当构造的时候会自动打印函数开始 
#endif
     }
     SAFE_BUFFER inline  ~AutoLog() noexcept {//当对象析构的时候会自动调用
#ifdef DEBUG_LOG
         auto end = Clock::now();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - timepoint).count();
         std::unique_lock<decltype(logmtx)> lock(logmtx);//加锁使得多线程下不会出现问题
         std::cout << funcname << xorstr("---> End Time: ")<<duration<<"ms\n";;//当对象析构时候自动打印结束
#endif
     }
 };
#define AUTOLOG  AutoLog log(xorstr(__FUNCTION__),"");//直接打印函数名字
#define AUTOLOGPERFIX(STR)  AutoLog log(xorstr(__FUNCTION__),STR);//打印函数名字但是加上一些后缀 __FUNCTION__是MSVC下的宏用于获取函数名字   MSVC的扩展都是双下划线开头的
#else
#define AUTOLOG             //release 模式下什么也不打印
#define AUTOLOGPERFIX(STR)
#endif
constexpr static std::size_t ROOTSEED = 0x92E2194B;//哈希的种子
namespace std {
    template<typename U>static inline  size_t hasher(const U& value) noexcept {//哈希器，用于对任意类型进行哈希
        //AUTOLOGPERFIX(typeid(U).name())//自动记录日志
        static std::hash<U> hasher;//生成一个hash对象
        return hasher(value);//调用hasher
    }
    template<typename... T>struct TupleHasher {
        static SAFE_BUFFER inline size_t hashvalue(const tuple<T...>& t) noexcept {//对于tuple的hash
            AUTOLOGPERFIX(typeid(tuple<T...>).name())//自动记录日志
            return hashImpl(t, index_sequence_for<T...>{}); //调用展开hashImpl
        }
        template<typename Tuple, size_t... I, typename expander = int[]>
        SAFE_BUFFER static inline size_t hashImpl(Tuple&& t, const index_sequence<I...>&, size_t seed = 0) noexcept {//对于tuple的hash的内部实现
            AUTOLOGPERFIX(typeid(Tuple).name())//自动记录日志
            (void)expander {0, ((seed ^= hasher(get<I>(std::forward<Tuple>(t))) + ROOTSEED + (seed << 6) + (seed >> 2)), 0)...};//异或哈希
            return seed;//返回seed seed是一个hash值
        }
        static SAFE_BUFFER inline size_t Seed()noexcept { AUTOLOG return ROOTSEED; }//返回内部的种子
    };
    template<typename... T>struct hash<tuple<T...>> {//对于tuple的hash 其中tuple内是任意的类型
        SAFE_BUFFER inline size_t operator()(const tuple<T...>& t) const noexcept {
            AUTOLOGPERFIX(std::string(xorstr("hash<> ")) + std::string(typeid(tuple<T...>).name()))//自动记录日志
            return TupleHasher<T...>::hashvalue(t); //调用hashvalue 获得hash值
        }
    };
    template <typename ...T>struct hash<std::function<T...>> {//对于std::function的hash
        SAFE_BUFFER inline  size_t operator()(const std::function<T...>& f) const noexcept {
            AUTOLOGPERFIX(std::string(xorstr("hash<> ")) + std::string(typeid(std::function<T...>).name()))//自动记录日志
            return hasher(f.target_type().name());//返回函数的类型的hash
        }
    };
}
namespace memoizationsearch {
    template<typename T>class SingleTon {
        SingleTon(const SingleTon&) = delete; // 删除拷贝构造函数
        SingleTon& operator=(const SingleTon&) = delete;
        SingleTon(SingleTon&&) = delete;      // 删除移动构造函数
        SingleTon& operator=(SingleTon&&) = delete;
        static std::atomic<T*> instancePtr;   // 使用 atomic 确保线程安全
        static std::once_flag initFlag;       // 用于保证线程安全
    protected:
        SingleTon() { AUTOLOGPERFIX(typeid(T).name()) }
        virtual ~SingleTon() {
            AUTOLOGPERFIX(typeid(T).name())// 自动记录日志
            delete instancePtr.load();        // 释放内存
            instancePtr.store(nullptr);       // 指针置空
        }
    public:
        template<typename... Args>
        SAFE_BUFFER inline static T& Construct(Args&&... args) noexcept {//构造对象
            AUTOLOGPERFIX(typeid(T).name())// 自动记录日志
            std::call_once(initFlag, [&] { instancePtr.store(new T(std::forward<Args>(args)...));});
            return *instancePtr.load();  // 返回对象的引用
        }
        SAFE_BUFFER inline static T& Construct() noexcept {//构造对象
            AUTOLOGPERFIX(typeid(T).name())// 自动记录日志
            std::call_once(initFlag, [&] { instancePtr.store(new T()); });//仅仅调用一次创建对象
            return *instancePtr.load();  // 返回对象的引用
        }
        SAFE_BUFFER inline static T& GetInstance() {
            AUTOLOGPERFIX(typeid(T).name())// 自动记录日志 
            auto ptr = Construct();
            if (!ptr)throw std::runtime_error(xorstr("Instance not yet constructed!"));//抛出未构造异常
            return *ptr;                     // 返回对象的引用
        }
    };
    template<typename T>std::atomic<T*> SingleTon<T>::instancePtr(nullptr);//初始化指针
    template<typename T>std::once_flag SingleTon<T>::initFlag;//初始化标志
    template<std::size_t... Indices>struct index_sequence {};
    template<std::size_t N, std::size_t... Indices>struct make_index_sequence : make_index_sequence<N - 1, N - 1, Indices...> {};
    template<std::size_t... Indices>struct make_index_sequence<0, Indices...> : index_sequence<Indices...> {};//生成一个序列
    //apply的内部实现
    template<typename F, typename Tuple, std::size_t... Indices>static SAFE_BUFFER auto inline ApplyImpl(F&& f, Tuple&& tuple, const index_sequence<Indices...>&)noexcept {
        AUTOLOGPERFIX(typeid(F).name())//自动记录日志
        return std::forward<F>(f)(std::get<Indices>(std::forward<Tuple>(tuple))...);//完美转发 调用f 将tuple的每个元素展开为参数
    }
    //apply等于std::apply 就是f(std::get<0>(tuple),std::get<1>(tuple),std::get<2>(tuple)...)的意思 会自动展开tuple把tuple的元素传递给f当参数
    template<typename F, typename Tuple, typename Index =make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value> >
    static SAFE_BUFFER inline auto Apply(F&& f, Tuple&& tuple)noexcept {
        AUTOLOGPERFIX(typeid(F).name())//自动记录日志
        static Index  sequence{};//生成一个序列
        return ApplyImpl(std::forward<F>(f), std::forward<Tuple>(tuple), sequence);//展开tuple
    }
   
    template<typename T>static inline bool ReadElementFromFile(std::ifstream& file, T& value) {//对于基本类型的读取
        AUTOLOGPERFIX(typeid(T).name())//自动记录日志
        if(!file) throw std::runtime_error(xorstr("File not open!"));//如果文件打开失败就抛出异常
        return file.read(reinterpret_cast<char*>((void*)&value), sizeof(value)).good();//读取文件
    }
    template<typename T>static inline bool WriteElementToFile(std::ofstream& file, const T& value) {//对于基本类型的写入
        AUTOLOGPERFIX(typeid(T).name())//自动记录日志
        if(!file) throw std::runtime_error(xorstr("File not open!"));//如果文件打开失败就抛出异常
        return file.write(reinterpret_cast<char*>((void*)&value), sizeof(value)).good();//写入文件
    }
    template<typename T, typename U>static inline bool WritePairToFile(std::ofstream& file, std::pair<T, U>& pair) { //对于pair的写入
        AUTOLOGPERFIX(typeid(std::pair<T, U>).name())////自动记录日志
        return !(!WriteElementToFile(file, pair.first) || !WriteElementToFile(file, pair.second)); //pair写入第一个和第二个元素
    }
    template<typename T, typename U>static inline bool ReadPairFromFile(std::ifstream& file, std::pair<T, U>& pair) { //对于pair的读取
        AUTOLOGPERFIX(typeid(std::pair<T, U>).name())//自动记录日志
        return !(!ReadElementFromFile(file, pair.first) || !ReadElementFromFile(file, pair.second)); //pair读取第一个和第二个元素
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if<I == sizeof...(Tp), bool>::type ReadTupleImpl(std::ifstream&, std::tuple<Tp...>&)noexcept {//对于tuple的读取
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//自动记录日志
        return true; //当I等于tuple的大小的时候返回true 递归的终止条件
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if < I<sizeof...(Tp), bool>::type ReadTupleImpl(std::ifstream& file, std::tuple<Tp...>& t)noexcept {//对于tuple的读取的内部实现
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//自动记录日志
        if (!ReadElementFromFile(file, std::get<I>(t)))return false;
        return ReadTupleImpl<I + 1, Tp...>(file, t);//递归处理tuple的每个元素
    }
    template<typename... Args>static inline bool ReadTupleFromFile(std::ifstream& file, std::tuple<Args...>& tuple) noexcept {//对于tuple的读取
        AUTOLOGPERFIX(typeid(std::tuple<Args...>).name())//自动记录日志
        return ReadTupleImpl(file, tuple); 
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if<I == sizeof...(Tp), bool>::type WriteTupleImpl(std::ofstream&, const std::tuple<Tp...>&) noexcept { //对于tuple的写入
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//自动记录日志
        return true; //当I等于tuple的大小的时候返回true 递归的终止条件
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if < I< sizeof...(Tp), bool>::type WriteTupleImpl(std::ofstream& file, const std::tuple<Tp...>& t) noexcept {//对于tuple的写入的内部实现
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//自动记录日志
        if (!WriteElementToFile(file, std::get<I>(t)))return false;
        return WriteTupleImpl<I + 1, Tp...>(file, t);//递归处理tuple的每个元素
    }
    template<typename... Args>SAFE_BUFFER static inline bool WriteTupleToFile(std::ofstream& file, const std::tuple<Args...>& tuple) noexcept { return WriteTupleImpl(file, tuple); }//对于tuple的写入
    template<>SAFE_BUFFER inline bool ReadElementFromFile<std::string>(std::ifstream& file, std::string& value) {//对于string的读取的特化
        AUTOLOGPERFIX(typeid(std::string).name())//自动记录日志
        if (!file) return false;//文件打开失败 
        std::size_t size = 0;//指明了字符串的长度
        if (!file.read(reinterpret_cast<char*>(&size), sizeof(size)))return false;//读取字符串的长度 
        std::string utf8String(size, '\0');//预留字符串的空间
        if (!file.read(&utf8String[0], size))return false;//读取字符串
        value = std::move(utf8String);//移动赋值给value
        return true;//返回成功
    }
    template<>SAFE_BUFFER inline bool ReadElementFromFile<std::wstring>(std::ifstream& file, std::wstring& value) {//对于wstring的读取的特化
        AUTOLOGPERFIX(typeid(std::wstring).name())//自动记录日志
        if (!file) return  false;//文件打开失败
        std::wstring::size_type len = 0;//指明了字符串的长度
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len)).good()) return false;//读取字符串的长度
        std::wstring wstr(len, L'\0');//预留字符串的空间
        if (!file.read(reinterpret_cast<char*>(&wstr[0]), len * sizeof(wchar_t)).good()) return false;//读取字符串
        value = std::move(wstr);
        return true;
    }
    template<>SAFE_BUFFER inline bool WriteElementToFile<std::string>(std::ofstream& file, const  std::string& value) {//对于string的写入的特化
        AUTOLOGPERFIX(typeid(std::string).name())//自动记录日志
        if (!file) return false;//文件打开失败
        std::size_t length = value.size();//字符串的长度
        if (length == 0) return false;//长度为0
        if (!file.write(reinterpret_cast<const char*>(&length), sizeof(length)))return false;//写入字符串的长度
        if (!file.write(value.data(), length))return false;//写入字符串
        return true;
    }
    template<>SAFE_BUFFER inline bool WriteElementToFile<std::wstring>(std::ofstream& file, const std::wstring& wstr) {//对于wstring的写入的特化
        AUTOLOGPERFIX(typeid(std::wstring).name())//自动记录日志
        if (!file) return  false;     //文件打开失败
        auto length = wstr.size();//字符串的长度
        if (!file.write(reinterpret_cast<const char*>(&length), sizeof(length)).good()) return false;//写入字符串的长度
        if (!file.write(reinterpret_cast<const char*>(wstr.c_str()), wstr.size() * sizeof(wchar_t))) return false;//写入字符串
        return true;
    }
    namespace nonstd {
        static std::recursive_mutex MemoizationGetCurrentTimeMtx;
        static auto program_start = std::chrono::high_resolution_clock ::now().time_since_epoch();//程序开始的时间的时间戳
        thread_local static TimeType count = 0;
        class ScopeLock {//一种自动加锁的RAII模式
            std::recursive_mutex& m_mutex;
            bool m_locked;
        public:
            SAFE_BUFFER explicit inline ScopeLock(std::recursive_mutex& mtx) : m_mutex(mtx), m_locked(false) { AUTOLOG m_locked = m_mutex.try_lock(); }
            SAFE_BUFFER inline ~ScopeLock() { AUTOLOG if (m_locked) m_mutex.unlock(); }
        };
        template<typename T>class ObjectPool :public SingleTon<ObjectPool<T>> {//单例奇异递归模板模式
            std::unordered_map<std::size_t, T*> m_pool;//一个哈希表存储对象的指针 
            unsigned char* objectpool;//对象池
            TimeType Size = 0;//对象池的大小
            TimeType offset = 0;//偏移 当有对象构造的时候会偏移
            std::recursive_mutex ObjectPoolMtx;//对象池的互斥量 但是是递归的
        public:
            inline ObjectPool() : offset(0) {
                AUTOLOG//自动记录日志
                Size = MAX_QUEUE_SIZE* sizeof(T);//默认的大小是10MB的大小 n个T类型对象总大小为4KB
                objectpool = new unsigned char[Size];//分配内存 分配失败new会自动抛出std::bad_alloc异常
                memset(objectpool, 0, Size);//初始化内存
                m_pool.reserve(Size);//map预留空间
            }
            ~ObjectPool() {
                AUTOLOG//自动记录日志
                ScopeLock lock(ObjectPoolMtx);//加锁
                m_pool.clear();//清空map
                delete[] objectpool;//所有在objectpool上的对象都是placement new的对象 需要手动释放
            }
            template<typename U, typename... Args>SAFE_BUFFER inline T& operator()(const U& prefix,const Args&... args){
                AUTOLOGPERFIX(std::string(xorstr("Functor ")) + typeid(T).name())//自动记录日志
                auto seed = typeid(T).hash_code() ^ std::hasher(std::make_tuple(prefix, args...));//通过对参数的hash和类型的hash异或得到一个唯一的seed
                auto it = m_pool.find(seed);//查找是否存在
                if (LIKELY(it != m_pool.end()))return *it->second;//如果存在直接返回
                if (offset + sizeof(T) > Size) {//如果空间不够
                    Size *=2;//扩大一倍
                    auto newpool = new unsigned char[Size];//分配新的空间
                    memcpy_s(newpool, Size, objectpool, Size / 2);//拷贝旧的数据
                    ScopeLock lock(ObjectPoolMtx);//加锁
                    for (auto& pair : m_pool)pair.second = reinterpret_cast<T*>(newpool + (reinterpret_cast<unsigned char*>(pair.second) - objectpool));//将原有的指针指向新的空间
                    delete[] objectpool;//释放旧的空间
                    objectpool = newpool;//指向新的空间
                }
                auto newObj = new (objectpool + offset) T(args...);//placement new//在指定的内存位置上构造对象
                offset += sizeof(T);//偏移 每次构造一个对象就偏移一个对象的大小
                std::thread([&]() {ScopeLock lock(ObjectPoolMtx);  m_pool[seed] = newObj; }).detach();//异步插入到map中
                return *newObj;//返回对象的指针的解引用
            }
        };
        SAFE_BUFFER static inline TimeType ApproximatelyGetCurrentTime() noexcept {
            //AUTOLOG //自动记录日志
            return std::chrono::duration_cast<std::chrono::microseconds>(program_start).count();//直接返回当前时间 program_start是程序开始的时间

        }
        template<typename T>
        SAFE_BUFFER static inline T safeadd(const T& a, const T& b) noexcept {//安全的相加不会溢出 最大只会返回INFINITYCACHE
            AUTOLOGPERFIX(typeid(T).name())//自动记录日志
            constexpr auto INFINITYVALUE = std::numeric_limits<T>::max();
            return (LIKELY(b > INFINITYVALUE - a)) ? INFINITYVALUE : a + b;
        }
        template<typename T> SAFE_BUFFER inline const T& Clamp(const T& m_value, const T& low, const T& high) noexcept { 
            AUTOLOGPERFIX(typeid(T).name())//自动记录日志
            return (m_value < low) ? low : (m_value > high) ? high : m_value; //返回m_value在low和high之间的值
        }
        static inline SAFE_BUFFER std::string SizeToString(size_t value, std::string result="") noexcept {//将size_t转换为字符串
            AUTOLOG//自动记录日志
            if (value == 0) return "0";//如果是0直接返回0
            while ((value/=10) > 0) {result = (char)('0' + (value % 10)) + result;}//取余一次相当于取一位
            return result;
        }
        static inline SAFE_BUFFER  std::string PathTruncate(const std::string& path) noexcept {//将路径截断为最大长度为MEMOIZATIONSEARCH
            AUTOLOG//自动记录日志
            std::string result = path;
            if (result.size() > MEMOIZATIONSEARCH)result.resize((TimeType)MEMOIZATIONSEARCH);
            result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char c) { return !std::isalnum(c) && c != '/'; }), result.end());
            std::replace(result.begin(), result.end(), '\\', '/');
            return result;
        }
        enum class CallType : unsigned char {
            cdeclcall = 0,//默认的调用约定
#ifdef _WIN64
            stdcall=1,//64位下的stdcall
            fastcall=2,//64位下的fastcall
#endif // _WIN64
        };
        struct CachedFunctionBase{
            mutable void* m_funcAddr;//函数的地址
            mutable CallType m_callType;//函数的调用约定
            mutable TimeType m_cacheTime;//缓存的时间
            mutable std::recursive_mutex m_mutex;//递归的互斥量
            inline  CachedFunctionBase() :m_funcAddr(nullptr), m_callType(CallType::cdeclcall), m_cacheTime(0) { AUTOLOG };//默认构造函数
            inline  CachedFunctionBase(void* funcAddr, CallType type, TimeType cacheTime = MEMOIZATIONSEARCH) : m_funcAddr(funcAddr), m_callType(type), m_cacheTime(cacheTime) {
                AUTOLOG
            }
			inline virtual ~CachedFunctionBase() { AUTOLOG }//析构函数自动记录日志，确保父类的析构函数被调用
            inline void SetCacheValidTime(TimeType cacheTime) noexcept {//设置缓存的有效时间
                AUTOLOG//自动记录日志
                m_cacheTime = cacheTime;//设置缓存的时间
            }//设置缓存的时间
            inline TimeType GetCacheTime() const { AUTOLOG  return m_cacheTime; }//获取缓存的时间
            inline operator void* () { AUTOLOG  return m_funcAddr; }//返回函数的地址
            inline bool operator==(const CachedFunctionBase& others) noexcept { AUTOLOG return m_funcAddr == others.m_funcAddr; }//比较两个元素的相等与否
            template<typename Func>inline bool operator==(Func&& f) noexcept { AUTOLOG  return m_funcAddr == &f; }//比较两个元素的相等与否
            inline bool operator!=(const CachedFunctionBase& others) noexcept { AUTOLOG return m_funcAddr != others.m_funcAddr; }//比较两个元素的相等与否
            template<typename Func>inline bool operator!=(Func&& f) noexcept { AUTOLOG  return m_funcAddr != &f; }//比较两个元素的相等与否
            inline operator bool()noexcept { AUTOLOG return (bool)m_funcAddr; }//判断是否为空
            inline auto Raw() const noexcept { AUTOLOG return m_funcAddr; }//返回函数的地址
            inline auto operator&() { AUTOLOG  return this->Raw(); }//重载&操作符 返回函数的地址
            //宏定义返回当前平台的名字
            SAFE_BUFFER inline std::string GetPlatformName() const noexcept {
                AUTOLOG//自动记录日志
                    //判断平台
#if defined(_NTDDK_)
                    return xorstr("Windows_Kernel");//Windows内核
#elif defined(_WIN64)
                    return xorstr("Windows_64-bit");//Windows 64位
#elif defined(_WIN32)
                    return xorstr("Windows_32-bit");//Windows 32位
#elif defined(__APPLE__) || defined(__MACH__)
                    return xorstr("Mac_OS");
#elif defined(__linux__)
                    return xorstr("Linux");
#elif defined(__unix__)
                    return xorstr("Unix");
#elif defined(__FreeBSD__)
                    return xorstr("FreeBSD");

#else
                    return "Unknown_Platform";//未知平台
#endif
            }
            //通过类型和文件名获取唯一的名字，szFileName是文件名 其他东西都是额外的
            template<typename T>SAFE_BUFFER inline std::string GetUniqueName(const T&, const char* szFileName) {
                AUTOLOG//自动记录日志
                static const auto&& platform = GetPlatformName();//获取平台的名字
                return nonstd::PathTruncate(szFileName + nonstd::SizeToString(typeid(T).hash_code())) + platform;//返回文件名加上类型的hash加上平台的名字 最大长度为MEMOIZATIONSEARCH
            }
            inline std::string GetObjectName() const noexcept {
                AUTOLOG//自动记录日志
                return typeid(CachedFunctionBase).name();//返回类型的名字
            }
        };
        //申明键的类型位可变的参数的tuple
        template<typename... Args>using KeyType = std::tuple<std::decay_t<Args>...>;
        //比较tuple内元素是否一一相等 递归具有短路逻辑 tuple内是任意的元素构成 按理来说只是为了函数参数的tuple包判断是否相等 参数应该不会太多而爆栈 递归本天成 迭代方是神，我是不想用递归的但是C++14标准的...包展开非常弱智要不然就只能全判断 完全没短路逻辑 更多写法探讨请自己摸索
        template<typename T>class is_hashable {//判断是否可以hash
            template<typename U>static auto test(int) -> decltype(std::hash<U>{}(std::declval<U>()), std::true_type{});
            template<typename>static std::false_type test(...) {};
        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };
        template <std::size_t I = 0, typename... Args, typename Tuple = std::tuple<Args...>>
        SAFE_BUFFER static inline bool const CompareTuple(Tuple&& tupA, Tuple&& tupB) noexcept {
            //AUTOLOGPERFIX(std::to_string(I))//日志打印自己是哪一个CompareTuple 方便知道自己是几层递归
            const auto& tupleA= std::forward<Tuple>(tupA);
            const auto& tupleB= std::forward<Tuple>(tupB);
            const auto& elemA = std::get<I>(tupleA);
            const auto& elemB = std::get<I>(tupleB);
            using ItemType = decltype(elemA);
            bool equal = false;
			if constexpr (std::is_arithmetic<ItemType>::value || !is_hashable<ItemType>::value) {//判断是否是基本型
                equal=(elemA == elemB);
            } else {
                equal=(std::hasher(elemA) == std::hasher(elemB));//非基本类型就对其求hash比较
            }
            if (LIKELY(equal)) {
                if constexpr (I + 1 < std::tuple_size<std::decay_t<Tuple>>::value) {//判断是不是最后一个
                    return CompareTuple<I + 1>(tupleA, tupleB);//非最后一个就递归
                }else {
                    return true;//少递归了一次 递归基
                }
            }
            return false;
        }
        //缓存的元素类型 由返回值和有效期组成的pair 有效期是一个返回值 有效期是一个时间戳 在这个时间戳之前是有效的
        template<typename R>using CacheitemType = std::pair<std::decay_t<R>, TimeType>;
        template<typename R, typename... Args>struct CachedFunction : public CachedFunctionBase {//具有参数的缓存函数是一个继承自CachedFunctionBase的类
            using   FuncType =  R(std::decay_t<Args>...);//声明函数类型
            mutable std::function<FuncType> m_Func;//函数对象
            using   ValueType = CacheitemType<R>;//缓存的元素类型
            using   ArgsType = KeyType<Args...>;
            using   CacheType = std::unordered_map<ArgsType, ValueType>;//一个哈希表存储缓存类型
            using   CallFuncType = std::function<bool(const R&, const ArgsType&)>;
            using   iterator = typename CacheType::iterator;//缓存迭代器的类型
            mutable std::unique_ptr<CacheType> m_Cache;//用智能指针 目的是为了不用手动释放内存 在对象析构的时候会自动释放
            mutable CacheType* m_CacheInstance = nullptr;//缓存的实例
            mutable iterator m_CacheEnd;//缓存的末尾迭代器
            mutable iterator m_StaticIter;//静态迭代器 用于缓存的查找 记录上一次的迭代器位置
            mutable std::unordered_map<HCALLBACK, CallFuncType> m_FilerCallBacks;
            mutable ArgsType m_StaticArgsTuple{};//记录上一次参数的tuple
            mutable bool m_FilterCacheStatus = true;
            mutable std::stack<R> m_oldResult;
            mutable std::unordered_map<ArgsType, std::pair<bool, TimeType>> m_FilterResultCache; // 缓存结果和时间戳
            inline  iterator begin() { AUTOLOG return m_Cache->begin(); }//返回缓存的开始迭代器 用于遍历
            inline  iterator end() { AUTOLOG return m_Cache->end(); }//返回缓存的末尾迭代器 用于遍历
            inline bool operator<(const CachedFunction& others) { AUTOLOG  return m_Cache->size() < others.m_Cache->size(); }//比较缓存大小
            inline bool operator>(const CachedFunction& others) { AUTOLOG  return m_Cache->size() > others.m_Cache->size(); }//比较缓存大小
            inline bool operator<=(const CachedFunction& others) { AUTOLOG  return m_Cache->size() <= others.m_Cache->size(); }//比较缓存大小
            inline auto Raw()const { AUTOLOG return m_funcAddr;}//返回函数指针但是lambda是没有函数指针的只有函数对象的地址
            inline auto operator&() const { AUTOLOG  return m_funcAddr; }//返回函数指针但是lambda是没有函数指针的只有函数对象的地址
            inline bool operator>=(const CachedFunction& others) { AUTOLOG return m_Cache->size() >= others.m_Cache->size();}//比较缓存大小
            //拷贝构造函数委托了父类的构造函数 并且拷贝了缓存
			inline bool& GetFilterCacheStatusRef() {//获取过滤回调是否缓存状态的引用 注意：设置为false虽然提高了实时性但是降低了可用性
                AUTOLOG//自动记录日志
                return m_FilterCacheStatus;
            }
            SAFE_BUFFER inline bool Filter(const R& value, const ArgsType& argsTuple, TimeType nowtime) noexcept {
                AUTOLOG//自动记录日志
				return (m_FilterCacheStatus) ? FilterCache(value, argsTuple, nowtime) : FilterNoCache(value, argsTuple);
            }
            SAFE_BUFFER inline bool FilterNoCache(const R& value, const ArgsType& argsTuple) noexcept {
                AUTOLOG//自动记录日志
                if (m_FilerCallBacks.empty()) return true; // 如果没有过滤回调，直接返回 true
                for (const auto& callback : m_FilerCallBacks) {
                    if (!callback.second(value, argsTuple)) return false;
                }
                return true;
            }
            SAFE_BUFFER inline bool FilterCache(const R& value, const ArgsType& argsTuple, TimeType nowtime) noexcept {
                AUTOLOG//自动记录日志
                if (m_FilerCallBacks.empty()) return true; // 如果没有过滤回调，直接返回 true
                // 检查缓存中是否已有结果
                auto it = m_FilterResultCache.find(argsTuple);
                if (it != m_FilterResultCache.end()) {
                    const auto& cached_result = it->second;
                    // 如果缓存未过期
                    if (cached_result.second > nowtime) {
                        return cached_result.first; // 返回缓存结果
                    }else {
                        m_FilterResultCache.erase(it); // 如果缓存过期，移除它
                    }
                }
                // 分别存储返回 false 的回调和 true 的回调
                std::list<CallFuncType> falseCallbacks;
                std::list<CallFuncType> trueCallbacks;
                // 获取所有回调的结果
                for (const auto& callback : m_FilerCallBacks) {
                    if (!callback.second(value, argsTuple)) {
                        falseCallbacks.push_back(callback.second); // 存储返回 false 的回调
                    }else {
                        trueCallbacks.push_back(callback.second); // 存储返回 true 的回调
                    }
                }
                // 尝试之前返回 false 的回调
                for (const auto& callback : falseCallbacks) {
                    if (callback(value, argsTuple)) {
                        // 存储结果为 true，并设置过期时间
                        m_FilterResultCache[argsTuple] = { true,safeadd<TimeType>(nowtime, m_cacheTime) };
                        return true;
                    }
                }
                // 如果没有找到 true 的结果，尝试之前返回 true 的回调
                for (const auto& callback : trueCallbacks) {
                    if (!callback(value, argsTuple)) {
                        // 存储结果为 false，并设置过期时间
                        m_FilterResultCache[argsTuple] = { false, safeadd<TimeType>(nowtime, m_cacheTime) };
                        return false;
                    }
                }
                // 存储结果为 true，并设置过期时间
                m_FilterResultCache[argsTuple] = { true, safeadd<TimeType>(nowtime, m_cacheTime) };
                return true;
            }
            [[nodiscard]] SAFE_BUFFER inline HCALLBACK AddFilterCallbacks(const CallFuncType& callbacks,bool bReserverOld=true)noexcept {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
				if (!callbacks) return INVALID_CALLBACK_HANDLE;//回调为空 callbacks是function对象
                HCALLBACK randnumber = INVALID_CALLBACK_HANDLE;
                do {
                    randnumber = getRandom<HCALLBACK>(0, INFINITYCACHE);
				} while (GetFilterCallbacks(randnumber) != m_FilerCallBacks.end());
                m_FilerCallBacks.insert(std::make_pair(randnumber,callbacks));
                if (UNLIKELY(!bReserverOld))ApplyFilter();
                return (HCALLBACK)randnumber;
            }
            SAFE_BUFFER inline bool DeleteFilterCallbacks(HCALLBACK& hcallback) noexcept {
                AUTOLOG; // 自动记录日志
                if (!ValidCallBackHandle(hcallback)) return false; // 返回 nullptr 表示无效回调
                ScopeLock lock(m_mutex); // 加锁保证线程安全
				auto it = GetFilterCallbacks(hcallback);//查找m_FilerCallBacks中的回调
                if (UNLIKELY(it == m_FilerCallBacks.end())) return false; // 返回 nullptr 表示未找到回调
                m_FilerCallBacks.erase(it); // 删除回调
                hcallback = INVALID_CALLBACK_HANDLE;
                return true; // 返回被删除的回调智能指针
            }
			SAFE_BUFFER inline bool ClearFilterCallbacks() noexcept {
				AUTOLOG; // 自动记录日志
				ScopeLock lock(m_mutex); // 加锁保证线程安全
                if (m_FilerCallBacks.empty()) return false;//回调列表为空返回false
				m_FilerCallBacks.clear(); // 清空所有回调
                return true;
			}
            SAFE_BUFFER inline bool ChangeFilterCallBacks(const HCALLBACK& hCallBack, const CallFuncType& newcallbacks,bool bReserveOld=true)noexcept {
                AUTOLOG//自动记录日志
                if (!ValidCallBackHandle(hCallBack)|| !newcallbacks) return false;//没有回调句柄
                ScopeLock lock(m_mutex);//加锁保证线程安全
				auto it = GetFilterCallbacks(hCallBack);//获取回调的迭代器
                if (it == m_FilerCallBacks.end()) return false;//没找到老的
                it->second = newcallbacks;
                if (UNLIKELY(!bReserveOld))ApplyFilter();
                return true;
            }
            SAFE_BUFFER inline void ApplyFilter() {
                AUTOLOG//自动记录日志
                std::async([this]() {
                    ScopeLock lock(m_mutex);//加锁保证线程安全
                    for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (!FilterNoCache(it->second.first, it->first)) ? m_Cache->erase(it) : ++it) {}
                });
            }
            SAFE_BUFFER inline auto GetFilterCallbacks(HCALLBACK hcallback)noexcept {
                AUTOLOG//自动记录日志
                if (!ValidCallBackHandle(hcallback)|| m_FilerCallBacks.empty()) return m_FilerCallBacks.end();//没有回调句柄
                return m_FilerCallBacks.find(hcallback);
            }
            SAFE_BUFFER inline auto& GetAllFilterCallBacksRef() {
                AUTOLOG//自动记录日志
                return m_FilerCallBacks;
            }
            SAFE_BUFFER inline CachedFunction(const CachedFunction& others) : CachedFunctionBase(others.m_funcAddr, others.m_callType, others.m_cacheTime), m_Func(others.m_Func), m_Cache(std::make_unique<CacheType>()) {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (others.m_Cache &&!others.m_Cache->empty())for (const auto& pair : *others.m_Cache)m_Cache->insert(pair);//拷贝所有的缓存
                if (LIKELY((bool)m_Cache)) {//如果缓存的内存不为空 
                    m_Cache->reserve((TimeType)MEMOIZATIONSEARCH);//预先分配空间
                    m_CacheEnd = m_Cache->end();//缓存的末尾迭代器
                    m_StaticIter = m_CacheEnd;//静态迭代器 上一次的迭代器位置默认是末尾
                    m_CacheInstance = m_Cache.get();//缓存的实例
                }
            }
            //获取缓存函数的名字
            inline std::string GetObjectName() const noexcept {
                AUTOLOG//自动记录日志
                return typeid(CachedFunction).name();//返回类型的名字
            }
            inline CachedFunction(const std::initializer_list<std::pair<ArgsType, ValueType>>& list) : CachedFunctionBase(nullptr, CallType::cdeclcall, MEMOIZATIONSEARCH), m_Cache(std::make_unique<CacheType>()) {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                for (auto& item : list)m_Cache->insert(item);
                if (LIKELY((bool)m_Cache)) {
                    m_Cache->reserve(MEMOIZATIONSEARCH);//预先分配空间
                    m_CacheEnd = m_Cache->end();//缓存的末尾迭代器
                    m_StaticIter = m_CacheEnd;//静态迭代器 上一次的迭代器位置默认是末尾
                    m_CacheInstance = m_Cache.get();//缓存的实例
                }
            }
            inline CachedFunction(CachedFunction&& other) noexcept : CachedFunctionBase(other.m_funcAddr, other.m_callType, other.m_cacheTime), m_Func(std::move(other.m_Func)) {
                AUTOLOG//自动记录日志
                m_Cache=std::move(other.m_Cache);
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (LIKELY((bool)m_Cache)) {//如果缓存的内存不为空
                    m_CacheEnd = m_Cache->end();//缓存的末尾迭代器
                    m_StaticIter = m_CacheEnd;//  静态迭代器 上一次的迭代器位置默认是末尾
                    m_CacheInstance = m_Cache.get();//缓存的实例
                }
            }
            inline ~CachedFunction() noexcept { AUTOLOG }
            inline CachedFunction& operator=(CachedFunction&& others)noexcept {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                m_Func = std::move(others.m_Func);//函数对象移动
                m_funcAddr = others.m_funcAddr;//函数的地址赋值
                others.m_funcAddr = 0;//置空
                if (LIKELY((bool)m_Cache)) {//如果缓存的内存不为空
                    m_CacheEnd = m_Cache->end();//缓存的末尾迭代器
                    m_StaticIter = m_CacheEnd;//静态迭代器 上一次的迭代器位置默认是末尾
                    m_CacheInstance = m_Cache.get();//缓存的实例
                    m_Cache = std::move(others.m_Cache);//缓存移动
                }
                return *this;
            }
            inline auto operator*()noexcept { return *m_Cache; }//返回缓存的引用
            inline CachedFunction& operator=(const CachedFunction& others)noexcept {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                m_Cache = std::make_unique<CacheType>();//重新分配内存
                m_Cache->reserve(others.m_Cache->size());//预先分配空间
                if (!others.m_Cache->empty())for (const auto& pair : *others.m_Cache) m_Cache->insert(pair);//拷贝所有的缓存到新的缓存
                m_Func = others.m_Func;//函数对象赋值 
                m_funcAddr = others.m_funcAddr;//函数的地址赋值
                if (LIKELY((bool)m_Cache)) {//如果缓存的内存不为空
                    m_CacheInstance = m_Cache.get();//缓存的实例
                    m_CacheEnd = m_Cache->end();//缓存的末尾迭代器
                    m_StaticIter = m_CacheEnd;//静态迭代器 上一次的迭代器位置默认是末尾
                }
                return *this;
            }
            inline operator std::function<FuncType>()noexcept { AUTOLOG return m_Func; }
            inline CachedFunction& operator+(const CachedFunction& others) noexcept {//合并缓存重载+操作符
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (!others.m_Cache->empty()) {//其他的缓存不为空
                    for (const auto& pair : *others.m_Cache){//遍历所有的缓存
                        if (m_Cache->find(pair.first) == m_Cache->end())m_Cache->insert(pair);//仅仅拷贝不存在的缓存
                    }
                }
                auto nowtime = ApproximatelyGetCurrentTime();//获取当前时间
                for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (it->second.second < nowtime) ? m_Cache->erase(it) : ++it) {}//遍历所有的缓存删除过期的缓存
                return *this;//返回自己
            }
            inline CachedFunction& operator+=(const CachedFunction& others) noexcept {//合并缓存重载+=操作符
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (!others.m_Cache->empty()){//其他的缓存不为空
                    for (const auto& pair : *others.m_Cache) {//遍历所有的缓存
                        auto it = m_Cache->find(pair.first);//查找是否存在
                        if (it == m_Cache->end())m_Cache->insert(pair);//仅仅拷贝不存在的缓存
                    }
                }
                //删除过期的缓存
                auto nowtime = ApproximatelyGetCurrentTime();//获取当前时间
                for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (it->second.second < nowtime) ? m_Cache->erase(it) : ++it) {}//遍历所有的缓存删除过期的缓存
                return *this;//返回自己
            }
            inline CachedFunction& operator-(const CachedFunction& others)noexcept {//删除缓存重载-操作符
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (!m_Cache->empty()){//自己的缓存不为空
                    for (const auto& pair : *m_Cache) {//遍历所有的缓存
                        auto it = others.m_Cache->find(pair.first);//查找是否存在
                        if (it != others.m_Cache->end())m_Cache->erase(pair.first);//仅仅删除存在的缓存
                    }
                }
                return *this;//返回自己
            }
            inline CachedFunction& operator-=(const CachedFunction& others) noexcept {//删除缓存重载-=操作符
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (!m_Cache->empty()){//自己的缓存不为空
                    for (const auto& pair : *m_Cache) {//遍历所有的缓存
                        auto it = others.m_Cache->find(pair.first);//查找是否存在
                        if (it != others.m_Cache->end())m_Cache->erase(pair.first);//仅仅删除存在的缓存
                    }
                }
                return *this;//返回自己
            }
            inline void SetCacheTime(TimeType cacheTime,bool reserveOld=true) noexcept {//设置缓存的时间
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁保证线程安全
                if (!reserveOld) {
                    cacheTime = nonstd::Clamp(cacheTime, (TimeType)1, INFINITYCACHE);//缓存的时间在1和INFINITYCACHE之间
                    for (auto& item : *m_Cache)item.second.second += cacheTime - m_cacheTime;//更新所有的缓存的时间
                }
                TimeType nowtime = ApproximatelyGetCurrentTime();//获取当前时间
                for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (it->second.second < nowtime) ? m_Cache->erase(it) : ++it) {}//删除过期的缓存
                m_cacheTime = cacheTime;//设置缓存的时间
            }
            inline CachedFunction(void* funcAddr, CallType type, const std::function<FuncType>& func, TimeType cacheTime = MEMOIZATIONSEARCH) : CachedFunctionBase(funcAddr, type, cacheTime), m_Func(std::move(func)) {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);
                m_Cache = std::make_unique<CacheType>();
                if (m_Cache) {
                    m_Cache->reserve((TimeType)MEMOIZATIONSEARCH);//预先分配空间
                    m_CacheEnd = m_Cache->end();
                    m_StaticIter = m_CacheEnd;
                    m_CacheInstance = m_Cache.get();
                }
            }
            SAFE_BUFFER inline R& AsyncsUpdateCache(const ArgsType& argsTuple)noexcept {//异步更新缓存
                AUTOLOG
                auto&& ret = Apply(m_Func, argsTuple);//调用函数
                ScopeLock lock(m_mutex);//加锁
                static auto&& Async= [this]()->void {
                    AUTOLOGPERFIX(xorstr("INSERT"))//自动记录日志
                    ScopeLock lock(m_mutex);//加锁
                    while (m_CacheInstance->load_factor() >= 0.75f)m_CacheInstance->reserve(m_CacheInstance->bucket_count() * 2);//负载因子大于0.75的时候扩容
                    m_CacheEnd = m_CacheInstance->end();//更新迭代器
				};
                std::async(Async);
                R*&& retref = nullptr;
                auto nowtime = ApproximatelyGetCurrentTime();
                m_CacheEnd = m_CacheInstance->end();//更新迭代器
                if (LIKELY(Filter(ret, argsTuple, nowtime))) {
                    auto iter= m_CacheInstance->insert_or_assign(argsTuple, ValueType{ ret, safeadd<TimeType>(nowtime,m_cacheTime+getRandom<TimeType>(0,m_cacheTime/2)) });//插入或者更新缓存
                    retref = &iter.first->second.first;
                    m_StaticIter = iter.first;
                }else {
                    m_oldResult.push(ret);//因为函数体可能递归 所以要临时存值
                    retref = &m_oldResult.top();
                    m_StaticIter = m_CacheEnd;
                }
                return *retref;//返回缓存的引用
            }
            SAFE_BUFFER inline R& operator()(const Args&... args)noexcept {
                TimeType&& m_nowtime = ApproximatelyGetCurrentTime();//获取当前时间
                ArgsType&& argsTuple = std::forward_as_tuple(std::cref(args)...);//构造参数的tuple
                if (LIKELY(m_CacheInstance->empty() ||!CompareTuple(m_StaticArgsTuple,argsTuple)|| m_StaticIter == m_CacheEnd)) {//前期不做时间判断
                    auto&& _staticiter = m_CacheInstance->find(argsTuple);//查找缓存
                    auto&& _m_cacheend = m_CacheInstance->end();//更新迭代器
                    ScopeLock lock(m_mutex);
                    m_StaticIter = _staticiter;
                    m_CacheEnd = _m_cacheend;
                    m_StaticArgsTuple = argsTuple; // 更新静态参数
                }
                if (LIKELY(m_StaticIter != m_CacheEnd)) {//如果找到了
                    auto&& valuepair = m_StaticIter->second;//获取缓存的值
                    if (LIKELY(valuepair.second > m_nowtime))return valuepair.first;//如果缓存的时间大于当前时间直接返回
                }
                if (m_oldResult.size()>1)m_oldResult.pop();
                return AsyncsUpdateCache(argsTuple); // 未找到则更新
            }
            inline void CleanCache() const noexcept { 
                AUTOLOG//自动记录日志
                std::thread([&]() {ScopeLock lock(m_mutex); m_Cache->clear(); }).detach();//异步清空缓存 加锁是为了防止多线程同时操作
            }
            inline void CleanCache(const ArgsType& parameters) const noexcept {//清空指定的缓存 指定的参数必须是tuple
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);
                auto it = m_Cache->find(parameters);
                if (LIKELY(it != m_Cache->end()))  m_Cache->erase(it->first);
            }
            inline void SetCacheResetTime(const ArgsType& parameters, TimeType cacheTime) const noexcept {//设置指定的参数的缓存的时间
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);
                auto it = m_Cache->find(parameters);
                if (LIKELY(it != m_Cache->end())) it->second = safeadd(ApproximatelyGetCurrentTime(), cacheTime);
            }
            inline bool SaveCache(const char* szFileName)noexcept {//将缓存保存到文件
                AUTOLOG//自动记录日志
                    std::ifstream file(GetUniqueName(m_Func, szFileName), std::ios::binary);//二进制形式打开文件
                operator<<(file);
                return true;
            }
            inline bool LoadCache(const char* szFileName)noexcept {//从文件加载缓存
                AUTOLOG//自动记录日志
                std::ifstream file(GetUniqueName(m_Func, szFileName), std::ios::binary);//二进制形式打开文件
                if (!operator<<(file)) return false;
                file.close();
                return true;
            }
            inline bool operator>>(std::ofstream& file) {//将缓存写入文件
                AUTOLOG//自动记录日志
                if (!file.is_open()) return false;//如果文件没有打开返回false
                ScopeLock lock(m_mutex);//加锁保证线程安全
                for (auto& pair : *m_Cache) {
                    if (std::hasher(pair.second.first) == std::hasher(R{})) continue;
                    if (!WriteTupleToFile(file, pair.first) || !WritePairToFile(file, pair.second)) {
                        continue;
                    }
                }
                file.flush();
                return true;
            }
            inline bool operator<<(std::ifstream& file) noexcept {//从文件读取缓存
                AUTOLOG//自动记录日志
                if (!file.is_open()) return false;//如果文件没有打开返回false
				if (file.tellg() == 0) return false;//如果文件的位置是0返回false
                ScopeLock lock(m_mutex);//加锁保证线程安全
                m_Cache->reserve((TimeType)MEMOIZATIONSEARCH);//预先分配空间
                while (file) {
                    ArgsType key{};//临时构造一个tuple
                    ValueType value{};//临时构造一个pair
                    if (!ReadTupleFromFile(file, key) || !ReadPairFromFile(file, value)) continue;//读取tuple和pair
                    if (value.second > ApproximatelyGetCurrentTime())m_Cache->insert(std::make_pair(key, value));//仅当缓存的时间大于当前时间的时候插入
                }
                for (auto it = m_Cache->begin(); it != m_Cache->end(); ) {
                    if (it->second.second < ApproximatelyGetCurrentTime()) {
                        it = m_Cache->erase(it); // erase returns the next valid iterator
                    }else {
                        ++it; // only increment if not erased
                    }
                }
                return true;
            }
            inline void operator>>(const CachedFunction& others) noexcept { //将自己的缓存插入到others的缓存中
                AUTOLOG//自动记录日志
                for (const auto& pair : *m_Cache)others.m_Cache->insert(pair); //插入到others的缓存中
            }
            inline void operator<<(const CachedFunction& others) noexcept {//将others的缓存插入到自己的缓存中
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);
                for (const auto& pair : *others.m_Cache)m_Cache->insert(pair);//插入到自己的缓存中
            }
            friend inline std::ostream& operator<<(std::ostream& os, const CachedFunction& func)noexcept {//重载输出流
                AUTOLOG//自动记录日志
                return  os<<func.m_Cache->size() << std::endl;//输出缓存的大小（个数）
            }
            inline std::function<FuncType>* operator->()noexcept { AUTOLOG return &m_Func; }//返回函数对象的指针
            inline bool empty()const noexcept { AUTOLOG return m_Cache->empty(); }//判断缓存是否为空
            inline std::size_t size()const noexcept { AUTOLOG return m_Cache->size(); }//返回缓存的大小
            SAFE_BUFFER inline iterator GetRef(const Args&...args)const noexcept {//重载下标操作符
                AUTOLOG//自动记录日志
                auto&& key = std::forward_as_tuple(std::cref(args)...);
                return  m_Cache->find(key);//返回内容但是不检查是否过期
            }
        };
        template<typename R> struct CachedFunction<R> : public CachedFunctionBase {//没有参数的缓存函数是一个继承自CachedFunctionBase的类
            mutable std::function<R()> m_Func;
            mutable CacheitemType<R> m_Cache;
            inline CachedFunction(const CacheitemType<R>& cache) :m_Func(nullptr), m_Cache(cache) { AUTOLOG }
            inline CachedFunction(void* funcAddr, CallType type, const std::function<R()>& func, TimeType cacheTime = MEMOIZATIONSEARCH) : CachedFunctionBase(funcAddr, type, cacheTime), m_Func(std::move(func)), m_Cache(std::make_pair(R{}, 0)) { AUTOLOG }
            inline R& setcacheresettime(const R& value, const  TimeType  cacheTime = ApproximatelyGetCurrentTime())const noexcept {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);
                m_Cache = std::make_pair(value, cacheTime);
                return m_Cache.first;
            }
            inline std::string GetObjectName() const noexcept {//获取对象的名字 类型的名字
                AUTOLOG//自动记录日志
                return typeid(CachedFunction).name();
            }
            inline operator bool()noexcept { AUTOLOG  return (bool)m_Func; }//检查函数是否有效
            inline operator std::function<R()>()noexcept { AUTOLOG  return m_Func; }
             inline CachedFunction(CachedFunction&& other) noexcept : CachedFunctionBase(other.m_funcAddr, other.m_callType, other.m_cacheTime), m_Func(std::move(other.m_Func)), m_Cache(std::move(other.m_Cache)) {
                AUTOLOG//自动记录日志
                m_funcAddr = std::move(other.m_funcAddr);
            }
            inline CachedFunction& operator=(CachedFunction&& others) noexcept {
                AUTOLOG//自动记录日志
                m_Func = std::move(others.m_Func);
                m_Cache = std::move(others.m_Cache);
                m_funcAddr = others.m_funcAddr;
                others.m_funcAddr = nullptr;
                return *this;
            }
            inline CachedFunction(const CachedFunction& others) :CachedFunctionBase(others.m_funcAddr, others.m_callType, others.m_cacheTime), m_Cache(others.m_Cache), m_Func(others.m_Func) { AUTOLOG }
            inline CachedFunction& operator=(const CachedFunction& others)noexcept {
                AUTOLOG//自动记录日志
                m_Cache = others.m_Cache;
                m_Func = others.m_Func;
                m_funcAddr = others.m_funcAddr;
                return *this;
            }
            inline bool LoadCache(const char* szFileName)noexcept {
                AUTOLOG//自动记录日志
                std::ifstream file(GetUniqueName(m_Func,szFileName), std::ios_base::binary);
                if (!file.is_open() || !operator<<(file)) return false;
                file.close();
                return true;
            }
            inline bool SaveCache(const char* szFileName) noexcept {
                AUTOLOG//自动记录日志
                std::ofstream file(GetUniqueName(m_Func,szFileName), std::ios::binary |std::ios::out|std::ios::trunc);
                if (!file.is_open() || !operator>>(file)) return false;
                file.close();
                return true;
            }
            inline bool operator >> (const std::ofstream& file) noexcept {
                AUTOLOG//自动记录日志
                if (!file.is_open()) return false;
                if (!WritePairToFile(file, m_Cache))return false;
                return true;
            }
            inline bool operator << (const std::ifstream& file) noexcept {
                AUTOLOG//自动记录日志
                if (!file.is_open()) return false;
                ScopeLock lock(m_mutex);
                if (!ReadPairFromFile(file, m_Cache))return false;
                auto nowtime = ApproximatelyGetCurrentTime();
                if (m_Cache.second > nowtime) m_Cache = std::make_pair(R{}, 0);
                return true;
            }
            inline void operator >> (const CachedFunction& others)noexcept { AUTOLOG others.m_Cache = m_Cache; }
            inline void operator << (const CachedFunction& others) noexcept { AUTOLOG m_Cache = others.m_Cache; }
            friend inline std::ostream& operator<<(std::ostream& os, const CachedFunction&)noexcept { AUTOLOG return os << 1; }
            SAFE_BUFFER inline R& operator()() const noexcept {
                AUTOLOGPERFIX(typeid(CachedFunction).name() + std::string(xorstr("Functor")))//自动记录日志
                if ((TimeType)m_Cache.second > (TimeType)ApproximatelyGetCurrentTime()) return m_Cache.first;
                return setcacheresettime(m_Func(),(safeadd(ApproximatelyGetCurrentTime(), m_cacheTime)));
            }
            inline void CleanCache()const noexcept {
                AUTOLOG//自动记录日志
                ScopeLock lock(m_mutex);//加锁
                m_Cache.second = ApproximatelyGetCurrentTime() - 1, m_Cache.first = R(); 
            }
             inline std::function<R()>* operator->()noexcept { AUTOLOG  return &m_Func; }
            inline bool empty()const noexcept { 
                AUTOLOG//自动记录日志
                return std::hash(m_Cache.first) == std::hash(R{}); //如果缓存的值是默认值则为空 
            }
            inline std::size_t size()const noexcept { 
                AUTOLOG//自动记录日志
                return 1; //函数只有一个缓存
            }
            inline CacheitemType<R>& GetRef() {
                return m_Cache;
            }
        };
        //萃取函数的返回值和参数
        template <typename F>struct function_traits : function_traits<decltype(&F::operator())> {};//当F可能是一个lambda表达式的时
        template <typename R, typename... Args>struct function_traits<R(*)(Args...)> {//函数指针的萃取，具有可变参数
            using return_type = R;
            using args_tuple_type = std::tuple<Args...>;
        };
        template <typename R>struct function_traits<R(*)()> {//函数指针的萃取，没有参数
            using return_type = R;
            using args_tuple_type = std::tuple<>;
        };
        template <typename R, typename... Args>struct function_traits<std::function<R(Args...)>> {//std::function的萃取，具有可变参数
            using return_type = R;
            using args_tuple_type = std::tuple<Args...>;
        };
        template <typename R>struct function_traits<std::function<R()>> {//std::function的萃取，没有参数
            using return_type = R;
            using args_tuple_type = std::tuple<>;
        };
        template <typename ClassType, typename R, typename... Args>
        struct function_traits<R(ClassType::*)(Args...) const> {//成员函数的萃取，具有可变参数
            using return_type = R;
            using args_tuple_type = std::tuple<Args...>;
        };
        template <typename ClassType, typename R>
        struct function_traits<R(ClassType::*)() const> {//成员函数的萃取，没有参数
            using return_type = R;
            using args_tuple_type = std::tuple<>;
        };
        template<typename ClassType, typename R, typename... Args>
        struct function_traits<R(ClassType::*)(Args...)> : function_traits<R(Args...)> {};//成员函数的萃取，具有可变参数
#ifdef _WIN64
        template <typename F>struct stdcallfunction_traits : stdcallfunction_traits<decltype(&F::operator())> {};//stdcall的lambda表达式萃取
        template <typename R, typename... Args>struct stdcallfunction_traits<R(STDCALL*)(Args...)> :public function_traits<R(STDCALL*)(Args...)> {};//stdcall的函数指针萃取，具有可变参数
        template <typename R>struct stdcallfunction_traits<R(STDCALL*)()> :public function_traits<R(STDCALL*)()> {};//stdcall的函数指针萃取，没有参数
        template <typename ClassType, typename R, typename... Args>struct stdcallfunction_traits<R(STDCALL ClassType::*)(Args...)> :public function_traits<R(STDCALL ClassType::*)(Args...)> {};//stdcall的成员函数萃取，具有可变参数
        template <typename ClassType, typename R>struct stdcallfunction_traits<R(STDCALL ClassType::*)()> :public function_traits<R(STDCALL ClassType::*)()> {};//stdcall的成员函数萃取，没有参数
        template <typename ClassType, typename R, typename... Args>struct stdcallfunction_traits<R(STDCALL ClassType::*)(Args...) const> :public function_traits<R(STDCALL ClassType::*)(Args...) const > {};//stdcall的成员函数萃取，具有可变参数
        template <typename ClassType, typename R>struct stdcallfunction_traits<R(STDCALL ClassType::*)() const> :public function_traits<R(STDCALL ClassType::*)() const > {};//stdcall的成员函数萃取，没有参数
        template <typename F>struct fastcallfunction_traits : fastcallfunction_traits<decltype(&F::operator())> {};//fastcall的lambda表达式萃取
        template <typename R, typename... Args>struct fastcallfunction_traits<R(FASTCALL*)(Args...)> :public function_traits<R(FASTCALL*)(Args...)> {};//fastcall的函数指针萃取，具有可变参数
        template <typename R>struct fastcallfunction_traits<R(FASTCALL*)()> :public function_traits<R(FASTCALL*)()> {};//fastcall的函数指针萃取，没有参数
        template <typename ClassType, typename R, typename... Args>struct fastcallfunction_traits<R(FASTCALL ClassType::*)(Args...)> :public function_traits<R(FASTCALL ClassType::*)(Args...)> {};//fastcall的成员函数萃取，具有可变参数
        template <typename ClassType, typename R>struct fastcallfunction_traits<R(FASTCALL ClassType::*)()> :public function_traits<R(FASTCALL ClassType::*)()> {};//fastcall的成员函数萃取，没有参数
        template <typename ClassType, typename R, typename... Args>struct fastcallfunction_traits<R(FASTCALL ClassType::*)(Args...) const> :public function_traits<R(FASTCALL ClassType::*)(Args...) const> {};//fastcall的成员函数萃取，具有可变参数
        template <typename ClassType, typename R>struct fastcallfunction_traits<R(FASTCALL ClassType::*)() const> :public function_traits <R(FASTCALL ClassType::*)() const> {};//fastcall的成员函数萃取，没有参数
#endif // _WIN64
        template <typename R, typename... Args> inline static auto& GetCachedFunction(void* funcAddr, CallType type, const std::function<R(Args...)>& func, TimeType cacheTime = MEMOIZATIONSEARCH) noexcept {
            AUTOLOGPERFIX(typeid(std::function<R(Args...)>).name())//自动记录日志
            return  ObjectPool<CachedFunction<R, Args...>>::Construct()(cacheTime, funcAddr, type, func, cacheTime);//根据参数和类型构造一个缓存函数
        }
        template <typename R> inline static auto GetCachedFunction(void* funcAddr, CallType type, const std::function<R()>& func, TimeType cacheTime = MEMOIZATIONSEARCH) noexcept {
            AUTOLOGPERFIX(typeid(std::function<R()>).name())//自动记录日志
            return  CachedFunction<R>(funcAddr, type, func, cacheTime);//根据参数和类型构造一个缓存函数
        }
        template<typename F, std::size_t... Is, typename DecayF = std::decay_t<F>> inline static auto MakeCachedImpl(F&& f, CallType type, TimeType validtime, const std::index_sequence<Is...>&) noexcept {
            AUTOLOGPERFIX(typeid(F).name())//自动记录日志
            using Functype = typename function_traits<DecayF>::return_type(typename std::tuple_element<Is, typename function_traits<DecayF>::args_tuple_type>::type...);
            return GetCachedFunction((void*)&f, type, std::function<Functype>(std::forward<F>(f)), validtime);//根据参数和类型构造一个缓存函数
        }
        template<typename T, typename R, typename... Args>SAFE_BUFFER inline auto ThisCall(T& obj, R(T::* func)(Args...))noexcept { 
            AUTOLOGPERFIX(typeid(T).name())//自动记录日志
            return [&obj, func](const Args&... args) -> R {return (obj.*func)(args...); }; //包装T类型的成员函数成为一个可调用的函数对象
        }
        template<typename T, typename F>inline auto ConvertMemberFunction(const T& obj, F&& func)noexcept { AUTOLOG return ThisCall(const_cast<T&>(obj), std::forward<F>(func)); }//转换成员函数成为一个可调用的函数对象
#ifdef _MSC_VER
#pragma  warning(  pop  )
#endif
    }
    template<typename T> using ArgsType_v = std::make_index_sequence<std::tuple_size<T>::value>;//通过tuple的大小生成一个index_sequence
}
//缓存函数的创建的加强版
template<typename F>inline  auto MakeCacheEx(F&& f, const  memoizationsearch::nonstd::CallType& calltype= memoizationsearch::nonstd::CallType::cdeclcall, TimeType _validtime=MEMOIZATIONSEARCH)noexcept {
    AUTOLOGPERFIX(typeid(F).name())
    using namespace memoizationsearch;
	using namespace memoizationsearch::nonstd;
    auto validtime =Clamp(_validtime, (TimeType)1, INFINITYCACHE);//有效时间 相加不会溢出 最大只会返回INFINITYCACHE
#ifdef _WIN64
    switch (calltype) {//根据不同的调用方式创建不同的缓存函数
    case  CallType::cdeclcall:break;
        //对于stdcall的特殊处理
    case  CallType::stdcall:  return  MakeCachedImpl(f, calltype, validtime, ArgsType_v<typename  stdcallfunction_traits<std::decay_t<F>>::args_tuple_type>{});
        //对于fastcall的特殊处理
    case  CallType::fastcall: return  MakeCachedImpl(f, calltype, validtime,ArgsType_v<typename  fastcallfunction_traits<std::decay_t<F>>::args_tuple_type>{});
    }
#endif // _WIN64
    return  MakeCachedImpl(f, calltype, validtime,ArgsType_v<typename function_traits<std::decay_t<F>>::args_tuple_type>{});//根据参数和类型构造一个缓存函数
}
template<typename R, typename...Args>  inline auto& MakeCacheItem(const R& ret, TimeType validtime, const Args&...args) noexcept { 
    AUTOLOG//自动记录日志
    return std::make_pair(std::make_tuple(args...), std::make_pair(ret, validtime)); 
}
template<typename F> inline auto MakeCache(F&& f, const TimeType validtime = MEMOIZATIONSEARCH) noexcept {//创建缓存 第二个参数是缓存的有效时间 默认是MEMOIZATIONSEARCH
    AUTOLOGPERFIX(typeid(F).name())//自动记录日志
    return MakeCacheEx(f, memoizationsearch::nonstd::CallType::cdeclcall, validtime);//有默认参数的函数写清楚是为了把validtime传递给他
}
template<typename T, class F> inline auto CacheMemberFunction(T&& obj, F&& func,const TimeType validtime = MEMOIZATIONSEARCH) noexcept { //缓存成员函数
    AUTOLOG//自动记录日志
    return MakeCacheEx(memoizationsearch::nonstd::ConvertMemberFunction(std::forward<T>(obj), std::forward<F>(func)), memoizationsearch::nonstd::CallType::cdeclcall, validtime);//先转换为一般的函数在调用,有默认参数的函数写清楚是为了把validtime传递给他
}
#endif

