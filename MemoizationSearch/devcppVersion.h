
#include <functional>
#include <unordered_map>
#include <chrono>
#include <type_traits>
#include <mutex>
#include <typeindex>
#include <random> 
#include <future>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <utility>
#undef _WIN32
#undef _WIN64
#ifndef MEMOIZATIONSEARCH
#define MEMOIZATIONSEARCH 260
using cachevalidtimeType = unsigned long long;
#define INFINITYCACHE static_cast<cachevalidtimeType>(0xffffffffffffffff)
using _clock = std::chrono::high_resolution_clock;
static auto program_start = std::chrono::high_resolution_clock::now();
static inline cachevalidtimeType GetTickCountChrono() noexcept { return (cachevalidtimeType)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - program_start).count(); }
template <bool _Test, class _Ty = void>
struct enable_if {}; // no member "type" when !_Test
template<bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#ifdef _WIN32
#pragma warning( push )
#pragma warning(disable: 26110)
#include <windows.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0000
#endif
#if _WIN32_WINNT >= 0x0600
#define _GetCurrentTime GetTickCount64
#define SAFE_BUFFER _declspec(safebuffers)
#else
#define SAFE_BUFFER
#define _GetCurrentTime GetTickCountChrono
#endif
#else
#define SAFE_BUFFER
#define _GetCurrentTime GetTickCountChrono
#endif
#undef max
template<typename T>struct remove_unsigned { using type = T; };
template<>struct remove_unsigned<cachevalidtimeType> { using type = long long; };
template<typename T>using remove_unsigned_t = typename remove_unsigned<T>::type;
#define SAFE_ADD_TWO(a, b) (((b) > (std::numeric_limits<remove_unsigned_t<cachevalidtimeType>>::max() - (a))) ? std::numeric_limits<remove_unsigned_t<cachevalidtimeType>>::max() : (a) + (b))
#define SAFE_LONG_ADD(a, b, ...) (SAFE_ADD_TWO(SAFE_ADD_TWO(a, b), __VA_ARGS__))
template <typename T>
struct is_pointer {
    static const bool value = false;
};
template <typename T>
struct is_pointer<T*> {
    static const bool value = true;
};
template<typename T> static inline bool ReadCacheFromFile(std::ifstream& file, T& value) {
    static_assert(std::is_standard_layout<typename std::decay<T>::type>::value || is_pointer<typename std::decay<T>::type>::value, "you need partial specialization for this type");
    return file.read(reinterpret_cast<char*>((void*)&value), sizeof(value)).good();
}
template<typename T>
static inline bool WriteCacheToFile(std::ofstream& file, T& value) {
    static_assert(std::is_standard_layout<typename std::decay<T>::type>::value ||
        is_pointer<typename std::decay<T>::type>::value,
        "you need partial specialization for this type");
    return file.write(reinterpret_cast<char*>((void*) & value), sizeof(value)).good();
}
template<typename T, typename U>static inline bool WritePairToFile(std::ofstream& file, std::pair<T, U>& pair) { return !(!WriteCacheToFile(file, pair.first) || !WriteCacheToFile(file, pair.second)); }
template<typename T, typename U>static inline bool ReadPairFromFile(std::ifstream& file, std::pair<T, U>& pair) { return !(!ReadCacheFromFile(file, pair.first) || !ReadCacheFromFile(file, pair.second)); }
template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if<I == sizeof...(Tp), bool>::type ReadTupleImpl(std::ifstream&, std::tuple<Tp...>&)noexcept { return true; }
template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if < I < sizeof...(Tp), bool>::type  ReadTupleImpl(std::ifstream& file, std::tuple<Tp...>& t)noexcept {
    if (!ReadCacheFromFile(file, std::get<I>(t)))return false;
    return ReadTupleImpl<I + 1, Tp...>(file, t);
}
template<typename... Args>static inline bool ReadTupleFromFile(std::ifstream& file, std::tuple<Args...>& tuple) noexcept { return ReadTupleImpl(file, tuple); }
template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if<I == sizeof...(Tp), bool>::type WriteTupleImpl(std::ofstream&, const std::tuple<Tp...>&) noexcept { return true; }
template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if < I < sizeof...(Tp), bool>::type
    WriteTupleImpl(std::ofstream& file, const std::tuple<Tp...>& t) noexcept {
    if (!WriteCacheToFile(file, std::get<I>(t)))return false;
    return WriteTupleImpl<I + 1, Tp...>(file, t);
}
template<typename... Args>static inline bool WriteTupleToFile(std::ofstream& file, const std::tuple<Args...>& tuple) noexcept { return WriteTupleImpl(file, tuple); }
static std::size_t rootseed = 0x92E2194B;
template<typename... T>
struct Hasher {
    static inline std::size_t hash_value(const std::tuple<T...>& t) noexcept {
        return hash_impl(t, std::integral_constant<std::size_t, sizeof...(T)>());
    }
    template<typename Tuple, std::size_t N>struct hash_impl_helper {
        static inline std::size_t apply(const Tuple& t, std::size_t seed) noexcept {
            seed ^= std::hash<typename std::tuple_element<N - 1, Tuple>::type>{}(std::get<N - 1>(t)) + rootseed + (seed << 6) + (seed >> 2);
            return hash_impl_helper<Tuple, N - 1>::apply(t, seed);
        }
    };
    template<typename Tuple>struct hash_impl_helper<Tuple, 0> { static inline std::size_t apply(const Tuple&, std::size_t seed) noexcept { return seed; } };
    template<typename Tuple, std::size_t N>static inline std::size_t hash_impl(const Tuple& t, std::integral_constant<std::size_t, N>, std::size_t seed = 0) noexcept { return hash_impl_helper<Tuple, N>::apply(t, seed); }
    static inline std::size_t& seed() noexcept { return rootseed; }
    static std::size_t rootseed;
};
template<typename T, typename... Args>typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type inline make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
template<typename T>typename std::enable_if<std::is_array<T>::value&& std::extent<T>::value == 0, std::unique_ptr<T>>::type inline  make_unique(std::size_t size) {
    using U = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new U[size]());
}
template<typename T, typename... Args>typename std::enable_if<std::extent<T>::value != 0, void>::type inline make_unique(Args&&...) = delete;
template<typename... T> std::size_t Hasher<T...>::rootseed = 0;  // Initialize the static member
namespace std {
    template<typename... T>
    struct hash<std::tuple<T...>> {
        inline std::size_t operator()(const std::tuple<T...>& t) const noexcept {
            return Hasher<T...>::hash_value(t);
        }
    };
}
namespace nonstd {
    template<typename T>constexpr inline const T& clamp(const T& m_value, const T& low, const T& high) noexcept { return (m_value < low) ? low : (m_value > high) ? high : m_value; }
    struct scope_lock {
        explicit scope_lock(std::recursive_mutex& mtx) : m_mutex(mtx), m_locked(false) { lock(); }
        ~scope_lock() { unlock(); }
        inline void lock() const noexcept { if (!m_locked && m_mutex.try_lock())m_locked = true; }
        inline void unlock()const noexcept { if (m_locked)m_mutex.unlock(), m_locked = false; }
        std::recursive_mutex& m_mutex;
        mutable std::atomic<bool> m_locked;
    };
    template<std::size_t... Indices>struct index_sequence {};
    template<std::size_t N, std::size_t... Indices>struct make_index_sequence : make_index_sequence<N - 1, N - 1, Indices...> {};
    template<std::size_t... Indices>struct make_index_sequence<0, Indices...> : index_sequence<Indices...> {};
    template<typename F, typename Tuple, std::size_t... Indices>
    auto inline apply_impl(F&& f, Tuple&& tuple, const index_sequence<Indices...>&) noexcept
        -> decltype(std::forward<F>(f)(std::get<Indices>(std::forward<Tuple>(tuple))...)) {
        return std::forward<F>(f)(std::get<Indices>(std::forward<Tuple>(tuple))...);
    }
    template<typename F, typename Tuple>auto apply(F&& f, Tuple&& tuple) noexcept
        -> decltype(apply_impl(std::forward<F>(f), std::forward<Tuple>(tuple),
            make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{})) {
        return apply_impl(std::forward<F>(f), std::forward<Tuple>(tuple),
            make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{});
    }
    static inline std::string sizeToString(size_t value) noexcept {
        if (value == 0) return "0";
        std::string result{};
        while (value > 0) {
            char digit = (char)('0' + (value % 10));
            result = digit + result;
            value /= 10;
        }
        return result;
    }
    static inline std::string path_truncate(const std::string& path) noexcept {
        std::string result = path;
        if (result.size() > MEMOIZATIONSEARCH)result.resize(MEMOIZATIONSEARCH);
        result.erase(std::remove_if(result.begin(), result.end(),
            [](unsigned char c) { return !std::isalnum(c) && c != '/'; }),
            result.end());
        std::replace(result.begin(), result.end(), '\\', '/');
        return result;
    }
    struct CachedFunctionBase {
        mutable void* m_funcAddr;
        mutable cachevalidtimeType m_cacheTime;
        mutable std::recursive_mutex m_mutex;
        mutable std::unique_ptr<std::mt19937> mt = make_unique<std::mt19937>((unsigned int)rootseed);
        mutable std::unique_ptr<std::uniform_int_distribution<int>> dist;
        inline  CachedFunctionBase() :m_funcAddr(nullptr), m_cacheTime(0) {};
        explicit inline  CachedFunctionBase(void* funcAddr, cachevalidtimeType cacheTime = MEMOIZATIONSEARCH) : m_funcAddr(funcAddr), m_cacheTime(cacheTime), dist(make_unique<std::uniform_int_distribution<int>>((int)0, clamp((int)(cacheTime >> 1), (int)0, std::numeric_limits<int>::max()))) {}
        inline void SetCacheTime(cachevalidtimeType cacheTime) noexcept { m_cacheTime = cacheTime; dist = make_unique<std::uniform_int_distribution<int>>((int)0, clamp((int)(cacheTime >> 1), 0, std::numeric_limits<int>::max())); }
        inline cachevalidtimeType GetCacheTime() { return m_cacheTime; }
        inline  cachevalidtimeType _getrandom()const noexcept { return (cachevalidtimeType)(*dist)(*mt.get()); }
        inline std::size_t seed() { return rootseed; }
        inline void setseed(std::size_t seed) { rootseed = seed; mt = make_unique<std::mt19937>((unsigned int)rootseed); }
        inline const void* operator&() { return m_funcAddr; }
        inline operator void* () { return m_funcAddr; }
        inline bool operator==(const CachedFunctionBase& others) noexcept { return m_funcAddr == others.m_funcAddr; }
        template<typename Func>inline bool operator==(Func&& f) noexcept { return m_funcAddr == &f; }
        inline bool operator!=(const CachedFunctionBase& others) noexcept { return m_funcAddr != others.m_funcAddr; }
        template<typename Func>inline bool operator!=(Func&& f) noexcept { return m_funcAddr != &f; }
    };
    template<typename R>using cacheitem_type = std::pair<typename std::decay<R>::type, cachevalidtimeType>;
    template<typename R, typename... Args>struct CachedFunction : public CachedFunctionBase {
        using func_type = R(typename std::decay<Args>::type...);
        mutable std::function<func_type> m_func;
        using key_type = std::tuple<typename std::decay<Args>::type...>;
        using value_type = cacheitem_type<R>;
        using cacheType = std::unordered_map<key_type, value_type>;
        mutable std::unique_ptr<cacheType> m_cache;
        inline typename cacheType::iterator begin() { return m_cache->begin(); }
        inline typename cacheType::iterator end() { return m_cache->end(); }
        inline operator bool()noexcept { return (bool)m_func; }
        inline bool operator<(const CachedFunction& others) { return m_cache->size() < others.m_cache->size(); }
        inline bool operator>(const CachedFunction& others) { return m_cache->size() > others.m_cache->size(); }
        inline bool operator<=(const CachedFunction& others) { return m_cache->size() <= others.m_cache->size(); }
        inline bool operator>=(const CachedFunction& others) { return m_cache->size() >= others.m_cache->size(); }
        inline CachedFunction(const CachedFunction& others) : CachedFunctionBase(others.m_funcAddr, others.m_cacheTime), m_cache(make_unique<cacheType>()), m_func(others.m_func) {
            if (!others.m_cache->empty())for (const auto& pair : *others.m_cache)m_cache->insert(pair);
        }
        inline CachedFunction(CachedFunction&& other) noexcept : CachedFunctionBase(other.m_funcAddr, other.m_cacheTime), m_func(std::move(other.m_func)), m_cache(std::move(other.m_cache)) {}
        inline CachedFunction& operator=(CachedFunction&& others)noexcept {
            m_func = std::move(others.m_func);
            m_cache = std::move(others.m_cache);
            m_funcAddr = others.m_funcAddr;
            others.m_funcAddr = 0;
            return *this;
        }
        inline std::unique_ptr<cacheType> operator*()noexcept { return *m_cache; }
        inline CachedFunction& operator=(const CachedFunction& others)noexcept {
            scope_lock lock(m_mutex);
            m_cache = make_unique<cacheType>();
            if (!others.m_cache->empty())for (const auto& pair : *others.m_cache) m_cache->insert(pair);
            m_func = others.m_func;
            m_funcAddr = others.m_funcAddr;
            return *this;
        }
        inline operator std::function<func_type>()noexcept { return m_func; }
        inline CachedFunction& operator+(const CachedFunction& others) noexcept {
            scope_lock lock(m_mutex);
            if (!others.m_cache->empty())for (const auto& pair : *others.m_cache)m_cache->insert(pair);
            return *this;
        }
        inline CachedFunction& operator+=(const CachedFunction& others) noexcept {
            scope_lock lock(m_mutex);
            if (!others.m_cache->empty())for (const auto& pair : *others.m_cache)m_cache->insert(pair);
            return *this;
        }
        inline CachedFunction& operator-(const CachedFunction& others)noexcept {
            scope_lock lock(m_mutex);
            for (const auto& pair : *m_cache) {
                auto it = others.m_cache->find(pair.first);
                if (it != others.m_cache->end())m_cache->erase(pair.first);
            }
            return *this;
        }
        inline CachedFunction& operator-=(const CachedFunction& others) noexcept {
            scope_lock lock(m_mutex);
            for (const auto& pair : *m_cache) {
                auto it = others.m_cache->find(pair.first);
                if (it != others.m_cache->end())m_cache->erase(pair.first);
            }
            return *this;
        }
        explicit inline CachedFunction(void* funcAddr, const std::function<func_type>& func, cachevalidtimeType cacheTime = MEMOIZATIONSEARCH) : CachedFunctionBase(funcAddr, cacheTime), m_func(std::move(func)) {
            scope_lock lock(m_mutex);
            m_cache = make_unique<cacheType>();
        }
        SAFE_BUFFER inline R& operator()(const Args&... args) const noexcept {
            key_type argsTuple(args...);
            auto now = _GetCurrentTime();
            auto it = m_cache->find(argsTuple);
            return (it != m_cache->end() && (cachevalidtimeType)it->second.second >= (cachevalidtimeType)now) ? it->second.first : AddCache(argsTuple, nonstd::apply(m_func, argsTuple), SAFE_LONG_ADD(now, m_cacheTime, _getrandom()));
        }
        inline R* seek(const Args&... args) noexcept {
            auto it = m_cache->find(key_type{ args... });
            return it != m_cache->end() ? &(it->second.first) : nullptr;
        }
        inline R& AddCache(const key_type& parameters, const R&& returnvalue, const cachevalidtimeType cacheTime) const noexcept {
            scope_lock lock(m_mutex);
            auto it = m_cache->find(parameters);
            if (it != m_cache->end()) {
                it->second = value_type{ returnvalue, cacheTime };
            }
            else {
                it = m_cache->emplace(parameters, value_type{ returnvalue, cacheTime }).first;
            }
            return it->second.first;
        }
        inline void ClearCache() const noexcept {
            scope_lock lock(m_mutex);
            m_cache->clear();
        }
        inline void ClearCache(const key_type& parameters) const noexcept {
            scope_lock lock(m_mutex);
            auto it = m_cache->find(parameters);
            if (it != m_cache->end())  m_cache->erase(it->first);
        }
        inline void SetCacheResetTime(const key_type& parameters, cachevalidtimeType cacheTime) const noexcept {
            scope_lock lock(m_mutex);
            auto it = m_cache->find(parameters);
            if (it != m_cache->end()) it->second = SAFE_ADD_TWO(_GetCurrentTime(), cacheTime);
        }
        inline bool savecache(const char* szFileName) {
            std::ofstream file(nonstd::path_truncate(szFileName + nonstd::sizeToString(typeid(decltype(m_func)).hash_code())), std::ios::binary | std::ios::trunc);
            if (!file.is_open()) throw std::runtime_error("file not found");
            bool bflag = true;
            for (auto& pair : *m_cache)if (!WriteTupleToFile(file, pair.first) || !WritePairToFile(file, pair.second)) bflag = false;
            file.close();
            return bflag;
        }
        inline bool loadcache(const char* szFileName)noexcept {
            std::ifstream file(nonstd::path_truncate(szFileName + nonstd::sizeToString(typeid(decltype(m_func)).hash_code())), std::ios::binary);
            if (!file.is_open()) return false;
            scope_lock lock(m_mutex);
            while (!file.eof()) {
                key_type key{};
                value_type value{};
                if (!ReadTupleFromFile(file, key) || !ReadPairFromFile(file, value)) break;
                m_cache->insert(std::make_pair(key, value));
            }
            file.close();
            return true;
        }
        inline void operator>>(std::ofstream& file) {
            if (!file.is_open()) throw std::runtime_error("file not found");
            for (auto& pair : *m_cache)if (!WriteTupleToFile(file, pair.first) || !WritePairToFile(file, pair.second))break;
        }
        inline void operator<<(std::ifstream& file) noexcept {
            if (!file.is_open()) return;
            scope_lock lock(m_mutex);
            while (file) {
                key_type key{};
                value_type value{};
                if (!ReadTupleFromFile(file, key) || !ReadPairFromFile(file, value)) break;
                m_cache->insert(std::make_pair(key, value));
            }
        }
        inline void operator>>(const CachedFunction& others) noexcept { for (const auto& pair : *m_cache)others.m_cache->insert(pair); }
        inline void operator<<(const CachedFunction& others) noexcept {
            scope_lock lock(m_mutex);
            for (const auto& pair : *others.m_cache)m_cache->insert(pair);
        }
        friend inline std::ostream& operator<<(std::ostream& os, const CachedFunction& func)noexcept { return os << func.m_cache->size() << std::endl; }
        inline std::function<func_type>* operator->()noexcept { return &m_func; }
        inline bool empty()const noexcept { return m_cache->empty(); }
        inline std::size_t size()const noexcept { return m_cache->size(); }
    };
    template<typename R> struct CachedFunction<R> : public CachedFunctionBase {
        mutable std::function<R()> m_func;
        using FuncType = decltype(m_func);
        mutable cacheitem_type<R> m_cache;
        explicit inline CachedFunction(void* funcAddr, const std::function<R()>& func, cachevalidtimeType cacheTime = MEMOIZATIONSEARCH) : CachedFunctionBase(funcAddr, cacheTime), m_func(std::move(func)), m_cache(std::make_pair(R{}, 0)) {}
        inline R& SetCacheResetTime(const R& value, const  cachevalidtimeType  cacheTime = _GetCurrentTime())const noexcept {
            scope_lock lock(m_mutex);
            m_cache = std::make_pair(value, cacheTime);
            return m_cache.first;
        }
        inline operator bool()noexcept { return (bool)m_func; }
        inline operator std::function<R()>()noexcept { return m_func; }
        inline CachedFunction(CachedFunction&& other) noexcept : CachedFunctionBase(other.m_funcAddr, other.m_cacheTime), m_func(std::move(other.m_func)), m_cache(std::move(other.m_cache)) {
            m_funcAddr = std::move(other.m_funcAddr);
        }
        inline CachedFunction& operator=(CachedFunction&& others) noexcept {
            m_func = std::move(others.m_func);
            m_cache = std::move(others.m_cache);
            m_funcAddr = others.m_funcAddr;
            others.m_funcAddr = nullptr;
            return *this;
        }
        inline CachedFunction(const CachedFunction& others) :m_cache(others.m_cache), m_func(others.m_func) {}
        inline CachedFunction& operator=(const CachedFunction& others)noexcept {
            m_cache = others.m_cache;
            m_func = others.m_func;
            m_funcAddr = others.m_funcAddr;
            return *this;
        }
        inline R* seek()const noexcept { return &m_cache.first; }
        inline bool loadcache(const char* szFileName)noexcept {
            std::ifstream file(nonstd::path_truncate(szFileName + nonstd::sizeToString(typeid(decltype(m_func)).hash_code())), std::ios_base::binary);
            if (!file.is_open()) return false;
            scope_lock lock(m_mutex);
            if (!ReadPairFromFile(file, m_cache)) return false;
            file.close();
            return true;
        }
        inline bool savecache(const char* szFileName) {
            std::ofstream file(nonstd::path_truncate(szFileName + nonstd::sizeToString(typeid(decltype(m_func)).hash_code())), std::ios::binary | std::ios::trunc);
            if (!file.is_open()) throw std::runtime_error("file not found");
            if (!WritePairToFile(file, m_cache)) return false;
            file.close();
            return true;
        }
        inline void operator >> (const std::ofstream& file) noexcept {
            if (!file.is_open()) return;
            if (!WritePairToFile(file, m_cache))return;
        }
        inline void operator << (const std::ifstream& file) noexcept {
            if (!file.is_open()) return;
            scope_lock lock(m_mutex);
            if (!ReadPairFromFile(file, m_cache))return;
        }
        inline void operator >> (const CachedFunction& others)noexcept { others.m_cache = m_cache; }
        inline void operator << (const CachedFunction& others) noexcept { m_cache = others.m_cache; }
        friend inline std::ostream& operator<<(std::ostream& os, const CachedFunction&)noexcept { return os << 1; }
        SAFE_BUFFER inline R& operator()() const noexcept {
            auto  now = _GetCurrentTime();
            if ((cachevalidtimeType)m_cache.second >= (cachevalidtimeType)now) return m_cache.first;
            return SetCacheResetTime(m_func(), (cachevalidtimeType)(SAFE_LONG_ADD(now, m_cacheTime, _getrandom())));
        }
        inline void ClearCache()const noexcept { m_cache.second = _GetCurrentTime() - 1, m_cache.first = R(); }
        inline std::function<R()>* operator->()noexcept { return &m_func; }
        inline bool empty()const noexcept { return m_cache.first == R{}; }
        inline std::size_t size()const noexcept { return 1; }
    };
    template <typename F>struct function_traits : function_traits<decltype(&F::operator())> {};
    template <typename R, typename... Args>struct function_traits<R(*)(Args...)> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename R, typename... Args>struct function_traits<std::function<R(Args...)>> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename ClassType, typename R, typename... Args>struct function_traits<R(ClassType::*)(Args...) const> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
#ifdef _WIN64
    template <typename F>struct stdcallfunction_traits : stdcallfunction_traits<decltype(&F::operator())> {};
    template <typename R, typename... Args>struct stdcallfunction_traits<R(__stdcall*)(Args...)> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename ClassType, typename R, typename... Args>struct stdcallfunction_traits<R(__stdcall ClassType::*)(Args...)> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename ClassType, typename R, typename... Args>struct stdcallfunction_traits<R(__stdcall ClassType::*)(Args...) const> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename F>struct fastcallfunction_traits : fastcallfunction_traits<decltype(&F::operator())> {};
    template <typename R, typename... Args>struct fastcallfunction_traits<R(__fastcall*)(Args...)> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename ClassType, typename R, typename... Args>struct fastcallfunction_traits<R(__fastcall ClassType::*)(Args...)> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
    template <typename ClassType, typename R, typename... Args>struct fastcallfunction_traits<R(__fastcall ClassType::*)(Args...) const> {
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
    };
#endif // _WIN64
    template <typename R, typename... Args>constexpr inline static CachedFunction<R, Args...> GetCachedFunction(void* funcAddr, const std::function<R(Args...)>& func, cachevalidtimeType cacheTime = MEMOIZATIONSEARCH) noexcept { return CachedFunction<R, Args...>(funcAddr, func, cacheTime); }
    template<typename F, std::size_t... Is>
    constexpr inline static auto makecached_impl(F&& f, cachevalidtimeType validtime, index_sequence<Is...>) noexcept
        -> CachedFunction<typename function_traits<typename std::decay<F>::type>::return_type,
        typename std::tuple_element<Is, typename function_traits<typename std::decay<F>::type>::args_tuple_type>::type...> {
        using DecayF = typename std::decay<F>::type;
        using Functype = typename function_traits<DecayF>::return_type(typename std::tuple_element<Is, typename function_traits<DecayF>::args_tuple_type>::type...);
        return GetCachedFunction((void*)&f, std::function<Functype>(std::forward<F>(f)), validtime);
    }
#ifdef WIN32
#pragma  warning(  pop  )
#endif
}
enum class CallType {
    cdeclcall = 0
#ifdef _WIN64
    , stdcall,
    fastcall
#endif // _WIN64
};
template<typename F>inline auto makecacheex(F&& f, const CallType& calltype = CallType::cdeclcall, cachevalidtimeType validtime = MEMOIZATIONSEARCH) noexcept
-> decltype(nonstd::makecached_impl(std::forward<F>(f), validtime, nonstd::make_index_sequence<std::tuple_size<typename nonstd::function_traits<typename std::decay<F>::type>::args_tuple_type>::value>{})) {
    auto _validtime = nonstd::clamp(validtime, 1ULL, INFINITYCACHE);
    switch (calltype) {
    case CallType::cdeclcall:
        return nonstd::makecached_impl(std::forward<F>(f), _validtime, nonstd::make_index_sequence<std::tuple_size<typename nonstd::function_traits<typename std::decay<F>::type>::args_tuple_type>::value>{});
#ifdef _WIN64
    case CallType::stdcall:
        return nonstd::makecached_impl(std::forward<F>(f), _validtime, nonstd::make_index_sequence<std::tuple_size<typename nonstd::function_traits<typename std::decay<F>::type>::args_tuple_type>::value>{});
    case CallType::fastcall:
        return nonstd::makecached_impl(std::forward<F>(f), _validtime, nonstd::make_index_sequence<std::tuple_size<typename nonstd::function_traits<typename std::decay<F>::type>::args_tuple_type>::value>{});
#endif // _WIN64
    }
    return nonstd::makecached_impl(std::forward<F>(f), _validtime, nonstd::make_index_sequence<std::tuple_size<typename nonstd::function_traits<typename std::decay<F>::type>::args_tuple_type>::value>{});
}
template<typename F, typename... Args>constexpr inline auto makecache(F&& f, const cachevalidtimeType validtime = MEMOIZATIONSEARCH) noexcept
-> decltype(makecacheex(std::forward<F>(f), CallType::cdeclcall, validtime)) {
    return makecacheex(std::forward<F>(f), CallType::cdeclcall, validtime);
}
#define max(a,b) (((a) > (b)) ? (a) : (b)) 
#endif
