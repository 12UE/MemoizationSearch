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
 * Project��https://github.com/12UE/MemoizationSearch
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
    static std::random_device rd;  // ���ڻ�ȡ�������
    static std::mt19937 gen(rd()); // Mersenne Twister ������
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dis(min, max);
        return dis(gen);
    }else {
        std::uniform_real_distribution<T> dis(min, max);
        return dis(gen);
    }
}
static constexpr size_t MAX_QUEUE_SIZE = 100; // �������ʵ���ֵ ������������
using TimeType = unsigned long long;
using HCALLBACK = TimeType;//�ص���� �����ֶ��ͷ�
#define INVALID_CALLBACK_HANDLE (HCALLBACK)-1//��Ч�Ļص����
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
float  MEMOIZATIONSEARCH = 75.0f;//Ĭ�ϵĻ�����Чʱ��75ms
float& GetGlobalDefaultCacheTimeRef() {//��ȡȫ�ֵĻ���ʱ�������
    return   MEMOIZATIONSEARCH;
}
#ifdef _WIN64

 auto INFINITYCACHE = static_cast<TimeType>(0x0000'FFFF'FFFF'FFFF'FFFFULL);//���建������ʱ��
#else
using TimeType = unsigned long;
 auto INFINITYCACHE = static_cast<TimeType>(0xFFFF'FFFF);//���建������ʱ����32λ��
#endif // _WIN64
#if defined(__GNUC__) || defined(__clang__)
#define LIKELY(x)   __builtin_expect((x), 1)//GCC/Clang�µ�likely
#define UNLIKELY(x)   __builtin_expect((x), 0)//GCC/Clang�µ�unlikely
#else
#define LIKELY(x)   (x)//MSVC��û���������
#define UNLIKELY(x) (x)//MSVC��û���������
#endif
#ifdef _MSC_VER
#pragma warning( push )//���浱ǰ�ľ���״̬
#pragma warning(disable:4984)//��ֹconstexpr��C++14֮ǰ�İ汾�ľ���
#define SAFE_BUFFER __declspec(safebuffers)//��MSVC�±�ʾ�����Ļ������ǰ�ȫ�� ������GCC����û�����������
#define CDECLCALL __cdecl//��MSVC�±�ʾ��������Լ����cdecl ������GCC����û�����������
#ifdef _WIN64
#define STDCALL __stdcall//������64λ��Windows�����������MSVC64λ�������¶���
#define FASTCALL __fastcall//������64λ��Windows�����������MSVC64λ�������¶���
#define THISCALL __thiscall//������64λ��Windows�����������MSVC64λ�������¶���
#endif
#else
#define SAFE_BUFFER//��GCC/Clang��û��������� ����Ϊ��
#define CDECLCALL//��GCC/Clang��û��������� ����Ϊ��
#define STDCALL//��GCC/Clang��û��������� ����Ϊ��
#define FASTCALL//��GCC/Clang��û��������� ����Ϊ��
#define THISCALL//��GCC/Clang��û��������� ����Ϊ��
#endif
#ifdef _DEBUG
 struct AutoLog {
     using Clock = std::chrono::high_resolution_clock;
     std::recursive_timed_mutex logmtx;
     Clock::time_point timepoint;
     std::string funcname;//�洢��������
     SAFE_BUFFER inline AutoLog(const std::string& szName = "", const std::string& perfix = "") noexcept {
         timepoint = Clock::now();
         std::unique_lock<decltype(logmtx)> lock(logmtx);//����ʹ�ö��߳��²����������
         funcname = szName + " " + perfix;//����������ƴ����ǰ׺
#ifdef DEBUG_LOG
         std::cout << funcname << xorstr("---> Begin\n");//�������ʱ����Զ���ӡ������ʼ 
#endif
     }
     SAFE_BUFFER inline  ~AutoLog() noexcept {//������������ʱ����Զ�����
#ifdef DEBUG_LOG
         auto end = Clock::now();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - timepoint).count();
         std::unique_lock<decltype(logmtx)> lock(logmtx);//����ʹ�ö��߳��²����������
         std::cout << funcname << xorstr("---> End Time: ")<<duration<<"ms\n";;//����������ʱ���Զ���ӡ����
#endif
     }
 };
#define AUTOLOG  AutoLog log(xorstr(__FUNCTION__),"");//ֱ�Ӵ�ӡ��������
#define AUTOLOGPERFIX(STR)  AutoLog log(xorstr(__FUNCTION__),STR);//��ӡ�������ֵ��Ǽ���һЩ��׺ __FUNCTION__��MSVC�µĺ����ڻ�ȡ��������   MSVC����չ����˫�»��߿�ͷ��
#else
#define AUTOLOG             //release ģʽ��ʲôҲ����ӡ
#define AUTOLOGPERFIX(STR)
#endif
constexpr static std::size_t ROOTSEED = 0x92E2194B;//��ϣ������
namespace std {
    template<typename U>static inline  size_t hasher(const U& value) noexcept {//��ϣ�������ڶ��������ͽ��й�ϣ
        //AUTOLOGPERFIX(typeid(U).name())//�Զ���¼��־
        static std::hash<U> hasher;//����һ��hash����
        return hasher(value);//����hasher
    }
    template<typename... T>struct TupleHasher {
        static SAFE_BUFFER inline size_t hashvalue(const tuple<T...>& t) noexcept {//����tuple��hash
            AUTOLOGPERFIX(typeid(tuple<T...>).name())//�Զ���¼��־
            return hashImpl(t, index_sequence_for<T...>{}); //����չ��hashImpl
        }
        template<typename Tuple, size_t... I, typename expander = int[]>
        SAFE_BUFFER static inline size_t hashImpl(Tuple&& t, const index_sequence<I...>&, size_t seed = 0) noexcept {//����tuple��hash���ڲ�ʵ��
            AUTOLOGPERFIX(typeid(Tuple).name())//�Զ���¼��־
            (void)expander {0, ((seed ^= hasher(get<I>(std::forward<Tuple>(t))) + ROOTSEED + (seed << 6) + (seed >> 2)), 0)...};//����ϣ
            return seed;//����seed seed��һ��hashֵ
        }
        static SAFE_BUFFER inline size_t Seed()noexcept { AUTOLOG return ROOTSEED; }//�����ڲ�������
    };
    template<typename... T>struct hash<tuple<T...>> {//����tuple��hash ����tuple�������������
        SAFE_BUFFER inline size_t operator()(const tuple<T...>& t) const noexcept {
            AUTOLOGPERFIX(std::string(xorstr("hash<> ")) + std::string(typeid(tuple<T...>).name()))//�Զ���¼��־
            return TupleHasher<T...>::hashvalue(t); //����hashvalue ���hashֵ
        }
    };
    template <typename ...T>struct hash<std::function<T...>> {//����std::function��hash
        SAFE_BUFFER inline  size_t operator()(const std::function<T...>& f) const noexcept {
            AUTOLOGPERFIX(std::string(xorstr("hash<> ")) + std::string(typeid(std::function<T...>).name()))//�Զ���¼��־
            return hasher(f.target_type().name());//���غ��������͵�hash
        }
    };
}
namespace memoizationsearch {
    template<typename T>class SingleTon {
        SingleTon(const SingleTon&) = delete; // ɾ���������캯��
        SingleTon& operator=(const SingleTon&) = delete;
        SingleTon(SingleTon&&) = delete;      // ɾ���ƶ����캯��
        SingleTon& operator=(SingleTon&&) = delete;
        static std::atomic<T*> instancePtr;   // ʹ�� atomic ȷ���̰߳�ȫ
        static std::once_flag initFlag;       // ���ڱ�֤�̰߳�ȫ
    protected:
        SingleTon() { AUTOLOGPERFIX(typeid(T).name()) }
        virtual ~SingleTon() {
            AUTOLOGPERFIX(typeid(T).name())// �Զ���¼��־
            delete instancePtr.load();        // �ͷ��ڴ�
            instancePtr.store(nullptr);       // ָ���ÿ�
        }
    public:
        template<typename... Args>
        SAFE_BUFFER inline static T& Construct(Args&&... args) noexcept {//�������
            AUTOLOGPERFIX(typeid(T).name())// �Զ���¼��־
            std::call_once(initFlag, [&] { instancePtr.store(new T(std::forward<Args>(args)...));});
            return *instancePtr.load();  // ���ض��������
        }
        SAFE_BUFFER inline static T& Construct() noexcept {//�������
            AUTOLOGPERFIX(typeid(T).name())// �Զ���¼��־
            std::call_once(initFlag, [&] { instancePtr.store(new T()); });//��������һ�δ�������
            return *instancePtr.load();  // ���ض��������
        }
        SAFE_BUFFER inline static T& GetInstance() {
            AUTOLOGPERFIX(typeid(T).name())// �Զ���¼��־ 
            auto ptr = Construct();
            if (!ptr)throw std::runtime_error(xorstr("Instance not yet constructed!"));//�׳�δ�����쳣
            return *ptr;                     // ���ض��������
        }
    };
    template<typename T>std::atomic<T*> SingleTon<T>::instancePtr(nullptr);//��ʼ��ָ��
    template<typename T>std::once_flag SingleTon<T>::initFlag;//��ʼ����־
    template<std::size_t... Indices>struct index_sequence {};
    template<std::size_t N, std::size_t... Indices>struct make_index_sequence : make_index_sequence<N - 1, N - 1, Indices...> {};
    template<std::size_t... Indices>struct make_index_sequence<0, Indices...> : index_sequence<Indices...> {};//����һ������
    //apply���ڲ�ʵ��
    template<typename F, typename Tuple, std::size_t... Indices>static SAFE_BUFFER auto inline ApplyImpl(F&& f, Tuple&& tuple, const index_sequence<Indices...>&)noexcept {
        AUTOLOGPERFIX(typeid(F).name())//�Զ���¼��־
        return std::forward<F>(f)(std::get<Indices>(std::forward<Tuple>(tuple))...);//����ת�� ����f ��tuple��ÿ��Ԫ��չ��Ϊ����
    }
    //apply����std::apply ����f(std::get<0>(tuple),std::get<1>(tuple),std::get<2>(tuple)...)����˼ ���Զ�չ��tuple��tuple��Ԫ�ش��ݸ�f������
    template<typename F, typename Tuple, typename Index =make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value> >
    static SAFE_BUFFER inline auto Apply(F&& f, Tuple&& tuple)noexcept {
        AUTOLOGPERFIX(typeid(F).name())//�Զ���¼��־
        static Index  sequence{};//����һ������
        return ApplyImpl(std::forward<F>(f), std::forward<Tuple>(tuple), sequence);//չ��tuple
    }
   
    template<typename T>static inline bool ReadElementFromFile(std::ifstream& file, T& value) {//���ڻ������͵Ķ�ȡ
        AUTOLOGPERFIX(typeid(T).name())//�Զ���¼��־
        if(!file) throw std::runtime_error(xorstr("File not open!"));//����ļ���ʧ�ܾ��׳��쳣
        return file.read(reinterpret_cast<char*>((void*)&value), sizeof(value)).good();//��ȡ�ļ�
    }
    template<typename T>static inline bool WriteElementToFile(std::ofstream& file, const T& value) {//���ڻ������͵�д��
        AUTOLOGPERFIX(typeid(T).name())//�Զ���¼��־
        if(!file) throw std::runtime_error(xorstr("File not open!"));//����ļ���ʧ�ܾ��׳��쳣
        return file.write(reinterpret_cast<char*>((void*)&value), sizeof(value)).good();//д���ļ�
    }
    template<typename T, typename U>static inline bool WritePairToFile(std::ofstream& file, std::pair<T, U>& pair) { //����pair��д��
        AUTOLOGPERFIX(typeid(std::pair<T, U>).name())////�Զ���¼��־
        return !(!WriteElementToFile(file, pair.first) || !WriteElementToFile(file, pair.second)); //pairд���һ���͵ڶ���Ԫ��
    }
    template<typename T, typename U>static inline bool ReadPairFromFile(std::ifstream& file, std::pair<T, U>& pair) { //����pair�Ķ�ȡ
        AUTOLOGPERFIX(typeid(std::pair<T, U>).name())//�Զ���¼��־
        return !(!ReadElementFromFile(file, pair.first) || !ReadElementFromFile(file, pair.second)); //pair��ȡ��һ���͵ڶ���Ԫ��
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if<I == sizeof...(Tp), bool>::type ReadTupleImpl(std::ifstream&, std::tuple<Tp...>&)noexcept {//����tuple�Ķ�ȡ
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//�Զ���¼��־
        return true; //��I����tuple�Ĵ�С��ʱ�򷵻�true �ݹ����ֹ����
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if < I<sizeof...(Tp), bool>::type ReadTupleImpl(std::ifstream& file, std::tuple<Tp...>& t)noexcept {//����tuple�Ķ�ȡ���ڲ�ʵ��
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//�Զ���¼��־
        if (!ReadElementFromFile(file, std::get<I>(t)))return false;
        return ReadTupleImpl<I + 1, Tp...>(file, t);//�ݹ鴦��tuple��ÿ��Ԫ��
    }
    template<typename... Args>static inline bool ReadTupleFromFile(std::ifstream& file, std::tuple<Args...>& tuple) noexcept {//����tuple�Ķ�ȡ
        AUTOLOGPERFIX(typeid(std::tuple<Args...>).name())//�Զ���¼��־
        return ReadTupleImpl(file, tuple); 
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if<I == sizeof...(Tp), bool>::type WriteTupleImpl(std::ofstream&, const std::tuple<Tp...>&) noexcept { //����tuple��д��
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//�Զ���¼��־
        return true; //��I����tuple�Ĵ�С��ʱ�򷵻�true �ݹ����ֹ����
    }
    template<std::size_t I = 0, typename... Tp>static inline typename std::enable_if < I< sizeof...(Tp), bool>::type WriteTupleImpl(std::ofstream& file, const std::tuple<Tp...>& t) noexcept {//����tuple��д����ڲ�ʵ��
        AUTOLOGPERFIX(typeid(std::tuple<Tp...>).name())//�Զ���¼��־
        if (!WriteElementToFile(file, std::get<I>(t)))return false;
        return WriteTupleImpl<I + 1, Tp...>(file, t);//�ݹ鴦��tuple��ÿ��Ԫ��
    }
    template<typename... Args>SAFE_BUFFER static inline bool WriteTupleToFile(std::ofstream& file, const std::tuple<Args...>& tuple) noexcept { return WriteTupleImpl(file, tuple); }//����tuple��д��
    template<>SAFE_BUFFER inline bool ReadElementFromFile<std::string>(std::ifstream& file, std::string& value) {//����string�Ķ�ȡ���ػ�
        AUTOLOGPERFIX(typeid(std::string).name())//�Զ���¼��־
        if (!file) return false;//�ļ���ʧ�� 
        std::size_t size = 0;//ָ�����ַ����ĳ���
        if (!file.read(reinterpret_cast<char*>(&size), sizeof(size)))return false;//��ȡ�ַ����ĳ��� 
        std::string utf8String(size, '\0');//Ԥ���ַ����Ŀռ�
        if (!file.read(&utf8String[0], size))return false;//��ȡ�ַ���
        value = std::move(utf8String);//�ƶ���ֵ��value
        return true;//���سɹ�
    }
    template<>SAFE_BUFFER inline bool ReadElementFromFile<std::wstring>(std::ifstream& file, std::wstring& value) {//����wstring�Ķ�ȡ���ػ�
        AUTOLOGPERFIX(typeid(std::wstring).name())//�Զ���¼��־
        if (!file) return  false;//�ļ���ʧ��
        std::wstring::size_type len = 0;//ָ�����ַ����ĳ���
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len)).good()) return false;//��ȡ�ַ����ĳ���
        std::wstring wstr(len, L'\0');//Ԥ���ַ����Ŀռ�
        if (!file.read(reinterpret_cast<char*>(&wstr[0]), len * sizeof(wchar_t)).good()) return false;//��ȡ�ַ���
        value = std::move(wstr);
        return true;
    }
    template<>SAFE_BUFFER inline bool WriteElementToFile<std::string>(std::ofstream& file, const  std::string& value) {//����string��д����ػ�
        AUTOLOGPERFIX(typeid(std::string).name())//�Զ���¼��־
        if (!file) return false;//�ļ���ʧ��
        std::size_t length = value.size();//�ַ����ĳ���
        if (length == 0) return false;//����Ϊ0
        if (!file.write(reinterpret_cast<const char*>(&length), sizeof(length)))return false;//д���ַ����ĳ���
        if (!file.write(value.data(), length))return false;//д���ַ���
        return true;
    }
    template<>SAFE_BUFFER inline bool WriteElementToFile<std::wstring>(std::ofstream& file, const std::wstring& wstr) {//����wstring��д����ػ�
        AUTOLOGPERFIX(typeid(std::wstring).name())//�Զ���¼��־
        if (!file) return  false;     //�ļ���ʧ��
        auto length = wstr.size();//�ַ����ĳ���
        if (!file.write(reinterpret_cast<const char*>(&length), sizeof(length)).good()) return false;//д���ַ����ĳ���
        if (!file.write(reinterpret_cast<const char*>(wstr.c_str()), wstr.size() * sizeof(wchar_t))) return false;//д���ַ���
        return true;
    }
    namespace nonstd {
        static std::recursive_mutex MemoizationGetCurrentTimeMtx;
        static auto program_start = std::chrono::high_resolution_clock ::now().time_since_epoch();//����ʼ��ʱ���ʱ���
        thread_local static TimeType count = 0;
        class ScopeLock {//һ���Զ�������RAIIģʽ
            std::recursive_mutex& m_mutex;
            bool m_locked;
        public:
            SAFE_BUFFER explicit inline ScopeLock(std::recursive_mutex& mtx) : m_mutex(mtx), m_locked(false) { AUTOLOG m_locked = m_mutex.try_lock(); }
            SAFE_BUFFER inline ~ScopeLock() { AUTOLOG if (m_locked) m_mutex.unlock(); }
        };
        template<typename T>class ObjectPool :public SingleTon<ObjectPool<T>> {//��������ݹ�ģ��ģʽ
            std::unordered_map<std::size_t, T*> m_pool;//һ����ϣ��洢�����ָ�� 
            unsigned char* objectpool;//�����
            TimeType Size = 0;//����صĴ�С
            TimeType offset = 0;//ƫ�� ���ж������ʱ���ƫ��
            std::recursive_mutex ObjectPoolMtx;//����صĻ����� �����ǵݹ��
        public:
            inline ObjectPool() : offset(0) {
                AUTOLOG//�Զ���¼��־
                Size = MAX_QUEUE_SIZE* sizeof(T);//Ĭ�ϵĴ�С��10MB�Ĵ�С n��T���Ͷ����ܴ�СΪ4KB
                objectpool = new unsigned char[Size];//�����ڴ� ����ʧ��new���Զ��׳�std::bad_alloc�쳣
                memset(objectpool, 0, Size);//��ʼ���ڴ�
                m_pool.reserve(Size);//mapԤ���ռ�
            }
            ~ObjectPool() {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(ObjectPoolMtx);//����
                m_pool.clear();//���map
                delete[] objectpool;//������objectpool�ϵĶ�����placement new�Ķ��� ��Ҫ�ֶ��ͷ�
            }
            template<typename U, typename... Args>SAFE_BUFFER inline T& operator()(const U& prefix,const Args&... args){
                AUTOLOGPERFIX(std::string(xorstr("Functor ")) + typeid(T).name())//�Զ���¼��־
                auto seed = typeid(T).hash_code() ^ std::hasher(std::make_tuple(prefix, args...));//ͨ���Բ�����hash�����͵�hash���õ�һ��Ψһ��seed
                auto it = m_pool.find(seed);//�����Ƿ����
                if (LIKELY(it != m_pool.end()))return *it->second;//�������ֱ�ӷ���
                if (offset + sizeof(T) > Size) {//����ռ䲻��
                    Size *=2;//����һ��
                    auto newpool = new unsigned char[Size];//�����µĿռ�
                    memcpy_s(newpool, Size, objectpool, Size / 2);//�����ɵ�����
                    ScopeLock lock(ObjectPoolMtx);//����
                    for (auto& pair : m_pool)pair.second = reinterpret_cast<T*>(newpool + (reinterpret_cast<unsigned char*>(pair.second) - objectpool));//��ԭ�е�ָ��ָ���µĿռ�
                    delete[] objectpool;//�ͷžɵĿռ�
                    objectpool = newpool;//ָ���µĿռ�
                }
                auto newObj = new (objectpool + offset) T(args...);//placement new//��ָ�����ڴ�λ���Ϲ������
                offset += sizeof(T);//ƫ�� ÿ�ι���һ�������ƫ��һ������Ĵ�С
                std::thread([&]() {ScopeLock lock(ObjectPoolMtx);  m_pool[seed] = newObj; }).detach();//�첽���뵽map��
                return *newObj;//���ض����ָ��Ľ�����
            }
        };
        SAFE_BUFFER static inline TimeType ApproximatelyGetCurrentTime() noexcept {
            //AUTOLOG //�Զ���¼��־
            return std::chrono::duration_cast<std::chrono::microseconds>(program_start).count();//ֱ�ӷ��ص�ǰʱ�� program_start�ǳ���ʼ��ʱ��

        }
        template<typename T>
        SAFE_BUFFER static inline T safeadd(const T& a, const T& b) noexcept {//��ȫ����Ӳ������ ���ֻ�᷵��INFINITYCACHE
            AUTOLOGPERFIX(typeid(T).name())//�Զ���¼��־
            constexpr auto INFINITYVALUE = std::numeric_limits<T>::max();
            return (LIKELY(b > INFINITYVALUE - a)) ? INFINITYVALUE : a + b;
        }
        template<typename T> SAFE_BUFFER inline const T& Clamp(const T& m_value, const T& low, const T& high) noexcept { 
            AUTOLOGPERFIX(typeid(T).name())//�Զ���¼��־
            return (m_value < low) ? low : (m_value > high) ? high : m_value; //����m_value��low��high֮���ֵ
        }
        static inline SAFE_BUFFER std::string SizeToString(size_t value, std::string result="") noexcept {//��size_tת��Ϊ�ַ���
            AUTOLOG//�Զ���¼��־
            if (value == 0) return "0";//�����0ֱ�ӷ���0
            while ((value/=10) > 0) {result = (char)('0' + (value % 10)) + result;}//ȡ��һ���൱��ȡһλ
            return result;
        }
        static inline SAFE_BUFFER  std::string PathTruncate(const std::string& path) noexcept {//��·���ض�Ϊ��󳤶�ΪMEMOIZATIONSEARCH
            AUTOLOG//�Զ���¼��־
            std::string result = path;
            if (result.size() > MEMOIZATIONSEARCH)result.resize((TimeType)MEMOIZATIONSEARCH);
            result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char c) { return !std::isalnum(c) && c != '/'; }), result.end());
            std::replace(result.begin(), result.end(), '\\', '/');
            return result;
        }
        enum class CallType : unsigned char {
            cdeclcall = 0,//Ĭ�ϵĵ���Լ��
#ifdef _WIN64
            stdcall=1,//64λ�µ�stdcall
            fastcall=2,//64λ�µ�fastcall
#endif // _WIN64
        };
        struct CachedFunctionBase{
            mutable void* m_funcAddr;//�����ĵ�ַ
            mutable CallType m_callType;//�����ĵ���Լ��
            mutable TimeType m_cacheTime;//�����ʱ��
            mutable std::recursive_mutex m_mutex;//�ݹ�Ļ�����
            inline  CachedFunctionBase() :m_funcAddr(nullptr), m_callType(CallType::cdeclcall), m_cacheTime(0) { AUTOLOG };//Ĭ�Ϲ��캯��
            inline  CachedFunctionBase(void* funcAddr, CallType type, TimeType cacheTime = MEMOIZATIONSEARCH) : m_funcAddr(funcAddr), m_callType(type), m_cacheTime(cacheTime) {
                AUTOLOG
            }
			inline virtual ~CachedFunctionBase() { AUTOLOG }//���������Զ���¼��־��ȷ���������������������
            inline void SetCacheValidTime(TimeType cacheTime) noexcept {//���û������Чʱ��
                AUTOLOG//�Զ���¼��־
                m_cacheTime = cacheTime;//���û����ʱ��
            }//���û����ʱ��
            inline TimeType GetCacheTime() const { AUTOLOG  return m_cacheTime; }//��ȡ�����ʱ��
            inline operator void* () { AUTOLOG  return m_funcAddr; }//���غ����ĵ�ַ
            inline bool operator==(const CachedFunctionBase& others) noexcept { AUTOLOG return m_funcAddr == others.m_funcAddr; }//�Ƚ�����Ԫ�ص�������
            template<typename Func>inline bool operator==(Func&& f) noexcept { AUTOLOG  return m_funcAddr == &f; }//�Ƚ�����Ԫ�ص�������
            inline bool operator!=(const CachedFunctionBase& others) noexcept { AUTOLOG return m_funcAddr != others.m_funcAddr; }//�Ƚ�����Ԫ�ص�������
            template<typename Func>inline bool operator!=(Func&& f) noexcept { AUTOLOG  return m_funcAddr != &f; }//�Ƚ�����Ԫ�ص�������
            inline operator bool()noexcept { AUTOLOG return (bool)m_funcAddr; }//�ж��Ƿ�Ϊ��
            inline auto Raw() const noexcept { AUTOLOG return m_funcAddr; }//���غ����ĵ�ַ
            inline auto operator&() { AUTOLOG  return this->Raw(); }//����&������ ���غ����ĵ�ַ
            //�궨�巵�ص�ǰƽ̨������
            SAFE_BUFFER inline std::string GetPlatformName() const noexcept {
                AUTOLOG//�Զ���¼��־
                    //�ж�ƽ̨
#if defined(_NTDDK_)
                    return xorstr("Windows_Kernel");//Windows�ں�
#elif defined(_WIN64)
                    return xorstr("Windows_64-bit");//Windows 64λ
#elif defined(_WIN32)
                    return xorstr("Windows_32-bit");//Windows 32λ
#elif defined(__APPLE__) || defined(__MACH__)
                    return xorstr("Mac_OS");
#elif defined(__linux__)
                    return xorstr("Linux");
#elif defined(__unix__)
                    return xorstr("Unix");
#elif defined(__FreeBSD__)
                    return xorstr("FreeBSD");

#else
                    return "Unknown_Platform";//δ֪ƽ̨
#endif
            }
            //ͨ�����ͺ��ļ�����ȡΨһ�����֣�szFileName���ļ��� �����������Ƕ����
            template<typename T>SAFE_BUFFER inline std::string GetUniqueName(const T&, const char* szFileName) {
                AUTOLOG//�Զ���¼��־
                static const auto&& platform = GetPlatformName();//��ȡƽ̨������
                return nonstd::PathTruncate(szFileName + nonstd::SizeToString(typeid(T).hash_code())) + platform;//�����ļ����������͵�hash����ƽ̨������ ��󳤶�ΪMEMOIZATIONSEARCH
            }
            inline std::string GetObjectName() const noexcept {
                AUTOLOG//�Զ���¼��־
                return typeid(CachedFunctionBase).name();//�������͵�����
            }
        };
        //������������λ�ɱ�Ĳ�����tuple
        template<typename... Args>using KeyType = std::tuple<std::decay_t<Args>...>;
        //�Ƚ�tuple��Ԫ���Ƿ�һһ��� �ݹ���ж�·�߼� tuple���������Ԫ�ع��� ������˵ֻ��Ϊ�˺���������tuple���ж��Ƿ���� ����Ӧ�ò���̫�����ջ �ݹ鱾��� �������������ǲ����õݹ�ĵ���C++14��׼��...��չ���ǳ�����Ҫ��Ȼ��ֻ��ȫ�ж� ��ȫû��·�߼� ����д��̽�����Լ�����
        template<typename T>class is_hashable {//�ж��Ƿ����hash
            template<typename U>static auto test(int) -> decltype(std::hash<U>{}(std::declval<U>()), std::true_type{});
            template<typename>static std::false_type test(...) {};
        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };
        template <std::size_t I = 0, typename... Args, typename Tuple = std::tuple<Args...>>
        SAFE_BUFFER static inline bool const CompareTuple(Tuple&& tupA, Tuple&& tupB) noexcept {
            //AUTOLOGPERFIX(std::to_string(I))//��־��ӡ�Լ�����һ��CompareTuple ����֪���Լ��Ǽ���ݹ�
            const auto& tupleA= std::forward<Tuple>(tupA);
            const auto& tupleB= std::forward<Tuple>(tupB);
            const auto& elemA = std::get<I>(tupleA);
            const auto& elemB = std::get<I>(tupleB);
            using ItemType = decltype(elemA);
            bool equal = false;
			if constexpr (std::is_arithmetic<ItemType>::value || !is_hashable<ItemType>::value) {//�ж��Ƿ��ǻ�����
                equal=(elemA == elemB);
            } else {
                equal=(std::hasher(elemA) == std::hasher(elemB));//�ǻ������;Ͷ�����hash�Ƚ�
            }
            if (LIKELY(equal)) {
                if constexpr (I + 1 < std::tuple_size<std::decay_t<Tuple>>::value) {//�ж��ǲ������һ��
                    return CompareTuple<I + 1>(tupleA, tupleB);//�����һ���͵ݹ�
                }else {
                    return true;//�ٵݹ���һ�� �ݹ��
                }
            }
            return false;
        }
        //�����Ԫ������ �ɷ���ֵ����Ч����ɵ�pair ��Ч����һ������ֵ ��Ч����һ��ʱ��� �����ʱ���֮ǰ����Ч��
        template<typename R>using CacheitemType = std::pair<std::decay_t<R>, TimeType>;
        template<typename R, typename... Args>struct CachedFunction : public CachedFunctionBase {//���в����Ļ��溯����һ���̳���CachedFunctionBase����
            using   FuncType =  R(std::decay_t<Args>...);//������������
            mutable std::function<FuncType> m_Func;//��������
            using   ValueType = CacheitemType<R>;//�����Ԫ������
            using   ArgsType = KeyType<Args...>;
            using   CacheType = std::unordered_map<ArgsType, ValueType>;//һ����ϣ��洢��������
            using   CallFuncType = std::function<bool(const R&, const ArgsType&)>;
            using   iterator = typename CacheType::iterator;//���������������
            mutable std::unique_ptr<CacheType> m_Cache;//������ָ�� Ŀ����Ϊ�˲����ֶ��ͷ��ڴ� �ڶ���������ʱ����Զ��ͷ�
            mutable CacheType* m_CacheInstance = nullptr;//�����ʵ��
            mutable iterator m_CacheEnd;//�����ĩβ������
            mutable iterator m_StaticIter;//��̬������ ���ڻ���Ĳ��� ��¼��һ�εĵ�����λ��
            mutable std::unordered_map<HCALLBACK, CallFuncType> m_FilerCallBacks;
            mutable ArgsType m_StaticArgsTuple{};//��¼��һ�β�����tuple
            mutable bool m_FilterCacheStatus = true;
            mutable std::stack<R> m_oldResult;
            mutable std::unordered_map<ArgsType, std::pair<bool, TimeType>> m_FilterResultCache; // ��������ʱ���
            inline  iterator begin() { AUTOLOG return m_Cache->begin(); }//���ػ���Ŀ�ʼ������ ���ڱ���
            inline  iterator end() { AUTOLOG return m_Cache->end(); }//���ػ����ĩβ������ ���ڱ���
            inline bool operator<(const CachedFunction& others) { AUTOLOG  return m_Cache->size() < others.m_Cache->size(); }//�Ƚϻ����С
            inline bool operator>(const CachedFunction& others) { AUTOLOG  return m_Cache->size() > others.m_Cache->size(); }//�Ƚϻ����С
            inline bool operator<=(const CachedFunction& others) { AUTOLOG  return m_Cache->size() <= others.m_Cache->size(); }//�Ƚϻ����С
            inline auto Raw()const { AUTOLOG return m_funcAddr;}//���غ���ָ�뵫��lambda��û�к���ָ���ֻ�к�������ĵ�ַ
            inline auto operator&() const { AUTOLOG  return m_funcAddr; }//���غ���ָ�뵫��lambda��û�к���ָ���ֻ�к�������ĵ�ַ
            inline bool operator>=(const CachedFunction& others) { AUTOLOG return m_Cache->size() >= others.m_Cache->size();}//�Ƚϻ����С
            //�������캯��ί���˸���Ĺ��캯�� ���ҿ����˻���
			inline bool& GetFilterCacheStatusRef() {//��ȡ���˻ص��Ƿ񻺴�״̬������ ע�⣺����Ϊfalse��Ȼ�����ʵʱ�Ե��ǽ����˿�����
                AUTOLOG//�Զ���¼��־
                return m_FilterCacheStatus;
            }
            SAFE_BUFFER inline bool Filter(const R& value, const ArgsType& argsTuple, TimeType nowtime) noexcept {
                AUTOLOG//�Զ���¼��־
				return (m_FilterCacheStatus) ? FilterCache(value, argsTuple, nowtime) : FilterNoCache(value, argsTuple);
            }
            SAFE_BUFFER inline bool FilterNoCache(const R& value, const ArgsType& argsTuple) noexcept {
                AUTOLOG//�Զ���¼��־
                if (m_FilerCallBacks.empty()) return true; // ���û�й��˻ص���ֱ�ӷ��� true
                for (const auto& callback : m_FilerCallBacks) {
                    if (!callback.second(value, argsTuple)) return false;
                }
                return true;
            }
            SAFE_BUFFER inline bool FilterCache(const R& value, const ArgsType& argsTuple, TimeType nowtime) noexcept {
                AUTOLOG//�Զ���¼��־
                if (m_FilerCallBacks.empty()) return true; // ���û�й��˻ص���ֱ�ӷ��� true
                // ��黺�����Ƿ����н��
                auto it = m_FilterResultCache.find(argsTuple);
                if (it != m_FilterResultCache.end()) {
                    const auto& cached_result = it->second;
                    // �������δ����
                    if (cached_result.second > nowtime) {
                        return cached_result.first; // ���ػ�����
                    }else {
                        m_FilterResultCache.erase(it); // ���������ڣ��Ƴ���
                    }
                }
                // �ֱ�洢���� false �Ļص��� true �Ļص�
                std::list<CallFuncType> falseCallbacks;
                std::list<CallFuncType> trueCallbacks;
                // ��ȡ���лص��Ľ��
                for (const auto& callback : m_FilerCallBacks) {
                    if (!callback.second(value, argsTuple)) {
                        falseCallbacks.push_back(callback.second); // �洢���� false �Ļص�
                    }else {
                        trueCallbacks.push_back(callback.second); // �洢���� true �Ļص�
                    }
                }
                // ����֮ǰ���� false �Ļص�
                for (const auto& callback : falseCallbacks) {
                    if (callback(value, argsTuple)) {
                        // �洢���Ϊ true�������ù���ʱ��
                        m_FilterResultCache[argsTuple] = { true,safeadd<TimeType>(nowtime, m_cacheTime) };
                        return true;
                    }
                }
                // ���û���ҵ� true �Ľ��������֮ǰ���� true �Ļص�
                for (const auto& callback : trueCallbacks) {
                    if (!callback(value, argsTuple)) {
                        // �洢���Ϊ false�������ù���ʱ��
                        m_FilterResultCache[argsTuple] = { false, safeadd<TimeType>(nowtime, m_cacheTime) };
                        return false;
                    }
                }
                // �洢���Ϊ true�������ù���ʱ��
                m_FilterResultCache[argsTuple] = { true, safeadd<TimeType>(nowtime, m_cacheTime) };
                return true;
            }
            [[nodiscard]] SAFE_BUFFER inline HCALLBACK AddFilterCallbacks(const CallFuncType& callbacks,bool bReserverOld=true)noexcept {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
				if (!callbacks) return INVALID_CALLBACK_HANDLE;//�ص�Ϊ�� callbacks��function����
                HCALLBACK randnumber = INVALID_CALLBACK_HANDLE;
                do {
                    randnumber = getRandom<HCALLBACK>(0, INFINITYCACHE);
				} while (GetFilterCallbacks(randnumber) != m_FilerCallBacks.end());
                m_FilerCallBacks.insert(std::make_pair(randnumber,callbacks));
                if (UNLIKELY(!bReserverOld))ApplyFilter();
                return (HCALLBACK)randnumber;
            }
            SAFE_BUFFER inline bool DeleteFilterCallbacks(HCALLBACK& hcallback) noexcept {
                AUTOLOG; // �Զ���¼��־
                if (!ValidCallBackHandle(hcallback)) return false; // ���� nullptr ��ʾ��Ч�ص�
                ScopeLock lock(m_mutex); // ������֤�̰߳�ȫ
				auto it = GetFilterCallbacks(hcallback);//����m_FilerCallBacks�еĻص�
                if (UNLIKELY(it == m_FilerCallBacks.end())) return false; // ���� nullptr ��ʾδ�ҵ��ص�
                m_FilerCallBacks.erase(it); // ɾ���ص�
                hcallback = INVALID_CALLBACK_HANDLE;
                return true; // ���ر�ɾ���Ļص�����ָ��
            }
			SAFE_BUFFER inline bool ClearFilterCallbacks() noexcept {
				AUTOLOG; // �Զ���¼��־
				ScopeLock lock(m_mutex); // ������֤�̰߳�ȫ
                if (m_FilerCallBacks.empty()) return false;//�ص��б�Ϊ�շ���false
				m_FilerCallBacks.clear(); // ������лص�
                return true;
			}
            SAFE_BUFFER inline bool ChangeFilterCallBacks(const HCALLBACK& hCallBack, const CallFuncType& newcallbacks,bool bReserveOld=true)noexcept {
                AUTOLOG//�Զ���¼��־
                if (!ValidCallBackHandle(hCallBack)|| !newcallbacks) return false;//û�лص����
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
				auto it = GetFilterCallbacks(hCallBack);//��ȡ�ص��ĵ�����
                if (it == m_FilerCallBacks.end()) return false;//û�ҵ��ϵ�
                it->second = newcallbacks;
                if (UNLIKELY(!bReserveOld))ApplyFilter();
                return true;
            }
            SAFE_BUFFER inline void ApplyFilter() {
                AUTOLOG//�Զ���¼��־
                std::async([this]() {
                    ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                    for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (!FilterNoCache(it->second.first, it->first)) ? m_Cache->erase(it) : ++it) {}
                });
            }
            SAFE_BUFFER inline auto GetFilterCallbacks(HCALLBACK hcallback)noexcept {
                AUTOLOG//�Զ���¼��־
                if (!ValidCallBackHandle(hcallback)|| m_FilerCallBacks.empty()) return m_FilerCallBacks.end();//û�лص����
                return m_FilerCallBacks.find(hcallback);
            }
            SAFE_BUFFER inline auto& GetAllFilterCallBacksRef() {
                AUTOLOG//�Զ���¼��־
                return m_FilerCallBacks;
            }
            SAFE_BUFFER inline CachedFunction(const CachedFunction& others) : CachedFunctionBase(others.m_funcAddr, others.m_callType, others.m_cacheTime), m_Func(others.m_Func), m_Cache(std::make_unique<CacheType>()) {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (others.m_Cache &&!others.m_Cache->empty())for (const auto& pair : *others.m_Cache)m_Cache->insert(pair);//�������еĻ���
                if (LIKELY((bool)m_Cache)) {//���������ڴ治Ϊ�� 
                    m_Cache->reserve((TimeType)MEMOIZATIONSEARCH);//Ԥ�ȷ���ռ�
                    m_CacheEnd = m_Cache->end();//�����ĩβ������
                    m_StaticIter = m_CacheEnd;//��̬������ ��һ�εĵ�����λ��Ĭ����ĩβ
                    m_CacheInstance = m_Cache.get();//�����ʵ��
                }
            }
            //��ȡ���溯��������
            inline std::string GetObjectName() const noexcept {
                AUTOLOG//�Զ���¼��־
                return typeid(CachedFunction).name();//�������͵�����
            }
            inline CachedFunction(const std::initializer_list<std::pair<ArgsType, ValueType>>& list) : CachedFunctionBase(nullptr, CallType::cdeclcall, MEMOIZATIONSEARCH), m_Cache(std::make_unique<CacheType>()) {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                for (auto& item : list)m_Cache->insert(item);
                if (LIKELY((bool)m_Cache)) {
                    m_Cache->reserve(MEMOIZATIONSEARCH);//Ԥ�ȷ���ռ�
                    m_CacheEnd = m_Cache->end();//�����ĩβ������
                    m_StaticIter = m_CacheEnd;//��̬������ ��һ�εĵ�����λ��Ĭ����ĩβ
                    m_CacheInstance = m_Cache.get();//�����ʵ��
                }
            }
            inline CachedFunction(CachedFunction&& other) noexcept : CachedFunctionBase(other.m_funcAddr, other.m_callType, other.m_cacheTime), m_Func(std::move(other.m_Func)) {
                AUTOLOG//�Զ���¼��־
                m_Cache=std::move(other.m_Cache);
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (LIKELY((bool)m_Cache)) {//���������ڴ治Ϊ��
                    m_CacheEnd = m_Cache->end();//�����ĩβ������
                    m_StaticIter = m_CacheEnd;//  ��̬������ ��һ�εĵ�����λ��Ĭ����ĩβ
                    m_CacheInstance = m_Cache.get();//�����ʵ��
                }
            }
            inline ~CachedFunction() noexcept { AUTOLOG }
            inline CachedFunction& operator=(CachedFunction&& others)noexcept {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                m_Func = std::move(others.m_Func);//���������ƶ�
                m_funcAddr = others.m_funcAddr;//�����ĵ�ַ��ֵ
                others.m_funcAddr = 0;//�ÿ�
                if (LIKELY((bool)m_Cache)) {//���������ڴ治Ϊ��
                    m_CacheEnd = m_Cache->end();//�����ĩβ������
                    m_StaticIter = m_CacheEnd;//��̬������ ��һ�εĵ�����λ��Ĭ����ĩβ
                    m_CacheInstance = m_Cache.get();//�����ʵ��
                    m_Cache = std::move(others.m_Cache);//�����ƶ�
                }
                return *this;
            }
            inline auto operator*()noexcept { return *m_Cache; }//���ػ��������
            inline CachedFunction& operator=(const CachedFunction& others)noexcept {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                m_Cache = std::make_unique<CacheType>();//���·����ڴ�
                m_Cache->reserve(others.m_Cache->size());//Ԥ�ȷ���ռ�
                if (!others.m_Cache->empty())for (const auto& pair : *others.m_Cache) m_Cache->insert(pair);//�������еĻ��浽�µĻ���
                m_Func = others.m_Func;//��������ֵ 
                m_funcAddr = others.m_funcAddr;//�����ĵ�ַ��ֵ
                if (LIKELY((bool)m_Cache)) {//���������ڴ治Ϊ��
                    m_CacheInstance = m_Cache.get();//�����ʵ��
                    m_CacheEnd = m_Cache->end();//�����ĩβ������
                    m_StaticIter = m_CacheEnd;//��̬������ ��һ�εĵ�����λ��Ĭ����ĩβ
                }
                return *this;
            }
            inline operator std::function<FuncType>()noexcept { AUTOLOG return m_Func; }
            inline CachedFunction& operator+(const CachedFunction& others) noexcept {//�ϲ���������+������
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (!others.m_Cache->empty()) {//�����Ļ��治Ϊ��
                    for (const auto& pair : *others.m_Cache){//�������еĻ���
                        if (m_Cache->find(pair.first) == m_Cache->end())m_Cache->insert(pair);//�������������ڵĻ���
                    }
                }
                auto nowtime = ApproximatelyGetCurrentTime();//��ȡ��ǰʱ��
                for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (it->second.second < nowtime) ? m_Cache->erase(it) : ++it) {}//�������еĻ���ɾ�����ڵĻ���
                return *this;//�����Լ�
            }
            inline CachedFunction& operator+=(const CachedFunction& others) noexcept {//�ϲ���������+=������
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (!others.m_Cache->empty()){//�����Ļ��治Ϊ��
                    for (const auto& pair : *others.m_Cache) {//�������еĻ���
                        auto it = m_Cache->find(pair.first);//�����Ƿ����
                        if (it == m_Cache->end())m_Cache->insert(pair);//�������������ڵĻ���
                    }
                }
                //ɾ�����ڵĻ���
                auto nowtime = ApproximatelyGetCurrentTime();//��ȡ��ǰʱ��
                for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (it->second.second < nowtime) ? m_Cache->erase(it) : ++it) {}//�������еĻ���ɾ�����ڵĻ���
                return *this;//�����Լ�
            }
            inline CachedFunction& operator-(const CachedFunction& others)noexcept {//ɾ����������-������
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (!m_Cache->empty()){//�Լ��Ļ��治Ϊ��
                    for (const auto& pair : *m_Cache) {//�������еĻ���
                        auto it = others.m_Cache->find(pair.first);//�����Ƿ����
                        if (it != others.m_Cache->end())m_Cache->erase(pair.first);//����ɾ�����ڵĻ���
                    }
                }
                return *this;//�����Լ�
            }
            inline CachedFunction& operator-=(const CachedFunction& others) noexcept {//ɾ����������-=������
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (!m_Cache->empty()){//�Լ��Ļ��治Ϊ��
                    for (const auto& pair : *m_Cache) {//�������еĻ���
                        auto it = others.m_Cache->find(pair.first);//�����Ƿ����
                        if (it != others.m_Cache->end())m_Cache->erase(pair.first);//����ɾ�����ڵĻ���
                    }
                }
                return *this;//�����Լ�
            }
            inline void SetCacheTime(TimeType cacheTime,bool reserveOld=true) noexcept {//���û����ʱ��
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                if (!reserveOld) {
                    cacheTime = nonstd::Clamp(cacheTime, (TimeType)1, INFINITYCACHE);//�����ʱ����1��INFINITYCACHE֮��
                    for (auto& item : *m_Cache)item.second.second += cacheTime - m_cacheTime;//�������еĻ����ʱ��
                }
                TimeType nowtime = ApproximatelyGetCurrentTime();//��ȡ��ǰʱ��
                for (auto it = m_Cache->begin(); it != m_Cache->end(); it = (it->second.second < nowtime) ? m_Cache->erase(it) : ++it) {}//ɾ�����ڵĻ���
                m_cacheTime = cacheTime;//���û����ʱ��
            }
            inline CachedFunction(void* funcAddr, CallType type, const std::function<FuncType>& func, TimeType cacheTime = MEMOIZATIONSEARCH) : CachedFunctionBase(funcAddr, type, cacheTime), m_Func(std::move(func)) {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);
                m_Cache = std::make_unique<CacheType>();
                if (m_Cache) {
                    m_Cache->reserve((TimeType)MEMOIZATIONSEARCH);//Ԥ�ȷ���ռ�
                    m_CacheEnd = m_Cache->end();
                    m_StaticIter = m_CacheEnd;
                    m_CacheInstance = m_Cache.get();
                }
            }
            SAFE_BUFFER inline R& AsyncsUpdateCache(const ArgsType& argsTuple)noexcept {//�첽���»���
                AUTOLOG
                auto&& ret = Apply(m_Func, argsTuple);//���ú���
                ScopeLock lock(m_mutex);//����
                static auto&& Async= [this]()->void {
                    AUTOLOGPERFIX(xorstr("INSERT"))//�Զ���¼��־
                    ScopeLock lock(m_mutex);//����
                    while (m_CacheInstance->load_factor() >= 0.75f)m_CacheInstance->reserve(m_CacheInstance->bucket_count() * 2);//�������Ӵ���0.75��ʱ������
                    m_CacheEnd = m_CacheInstance->end();//���µ�����
				};
                std::async(Async);
                R*&& retref = nullptr;
                auto nowtime = ApproximatelyGetCurrentTime();
                m_CacheEnd = m_CacheInstance->end();//���µ�����
                if (LIKELY(Filter(ret, argsTuple, nowtime))) {
                    auto iter= m_CacheInstance->insert_or_assign(argsTuple, ValueType{ ret, safeadd<TimeType>(nowtime,m_cacheTime+getRandom<TimeType>(0,m_cacheTime/2)) });//������߸��»���
                    retref = &iter.first->second.first;
                    m_StaticIter = iter.first;
                }else {
                    m_oldResult.push(ret);//��Ϊ��������ܵݹ� ����Ҫ��ʱ��ֵ
                    retref = &m_oldResult.top();
                    m_StaticIter = m_CacheEnd;
                }
                return *retref;//���ػ��������
            }
            SAFE_BUFFER inline R& operator()(const Args&... args)noexcept {
                TimeType&& m_nowtime = ApproximatelyGetCurrentTime();//��ȡ��ǰʱ��
                ArgsType&& argsTuple = std::forward_as_tuple(std::cref(args)...);//���������tuple
                if (LIKELY(m_CacheInstance->empty() ||!CompareTuple(m_StaticArgsTuple,argsTuple)|| m_StaticIter == m_CacheEnd)) {//ǰ�ڲ���ʱ���ж�
                    auto&& _staticiter = m_CacheInstance->find(argsTuple);//���һ���
                    auto&& _m_cacheend = m_CacheInstance->end();//���µ�����
                    ScopeLock lock(m_mutex);
                    m_StaticIter = _staticiter;
                    m_CacheEnd = _m_cacheend;
                    m_StaticArgsTuple = argsTuple; // ���¾�̬����
                }
                if (LIKELY(m_StaticIter != m_CacheEnd)) {//����ҵ���
                    auto&& valuepair = m_StaticIter->second;//��ȡ�����ֵ
                    if (LIKELY(valuepair.second > m_nowtime))return valuepair.first;//��������ʱ����ڵ�ǰʱ��ֱ�ӷ���
                }
                if (m_oldResult.size()>1)m_oldResult.pop();
                return AsyncsUpdateCache(argsTuple); // δ�ҵ������
            }
            inline void CleanCache() const noexcept { 
                AUTOLOG//�Զ���¼��־
                std::thread([&]() {ScopeLock lock(m_mutex); m_Cache->clear(); }).detach();//�첽��ջ��� ������Ϊ�˷�ֹ���߳�ͬʱ����
            }
            inline void CleanCache(const ArgsType& parameters) const noexcept {//���ָ���Ļ��� ָ���Ĳ���������tuple
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);
                auto it = m_Cache->find(parameters);
                if (LIKELY(it != m_Cache->end()))  m_Cache->erase(it->first);
            }
            inline void SetCacheResetTime(const ArgsType& parameters, TimeType cacheTime) const noexcept {//����ָ���Ĳ����Ļ����ʱ��
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);
                auto it = m_Cache->find(parameters);
                if (LIKELY(it != m_Cache->end())) it->second = safeadd(ApproximatelyGetCurrentTime(), cacheTime);
            }
            inline bool SaveCache(const char* szFileName)noexcept {//�����汣�浽�ļ�
                AUTOLOG//�Զ���¼��־
                    std::ifstream file(GetUniqueName(m_Func, szFileName), std::ios::binary);//��������ʽ���ļ�
                operator<<(file);
                return true;
            }
            inline bool LoadCache(const char* szFileName)noexcept {//���ļ����ػ���
                AUTOLOG//�Զ���¼��־
                std::ifstream file(GetUniqueName(m_Func, szFileName), std::ios::binary);//��������ʽ���ļ�
                if (!operator<<(file)) return false;
                file.close();
                return true;
            }
            inline bool operator>>(std::ofstream& file) {//������д���ļ�
                AUTOLOG//�Զ���¼��־
                if (!file.is_open()) return false;//����ļ�û�д򿪷���false
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                for (auto& pair : *m_Cache) {
                    if (std::hasher(pair.second.first) == std::hasher(R{})) continue;
                    if (!WriteTupleToFile(file, pair.first) || !WritePairToFile(file, pair.second)) {
                        continue;
                    }
                }
                file.flush();
                return true;
            }
            inline bool operator<<(std::ifstream& file) noexcept {//���ļ���ȡ����
                AUTOLOG//�Զ���¼��־
                if (!file.is_open()) return false;//����ļ�û�д򿪷���false
				if (file.tellg() == 0) return false;//����ļ���λ����0����false
                ScopeLock lock(m_mutex);//������֤�̰߳�ȫ
                m_Cache->reserve((TimeType)MEMOIZATIONSEARCH);//Ԥ�ȷ���ռ�
                while (file) {
                    ArgsType key{};//��ʱ����һ��tuple
                    ValueType value{};//��ʱ����һ��pair
                    if (!ReadTupleFromFile(file, key) || !ReadPairFromFile(file, value)) continue;//��ȡtuple��pair
                    if (value.second > ApproximatelyGetCurrentTime())m_Cache->insert(std::make_pair(key, value));//���������ʱ����ڵ�ǰʱ���ʱ�����
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
            inline void operator>>(const CachedFunction& others) noexcept { //���Լ��Ļ�����뵽others�Ļ�����
                AUTOLOG//�Զ���¼��־
                for (const auto& pair : *m_Cache)others.m_Cache->insert(pair); //���뵽others�Ļ�����
            }
            inline void operator<<(const CachedFunction& others) noexcept {//��others�Ļ�����뵽�Լ��Ļ�����
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);
                for (const auto& pair : *others.m_Cache)m_Cache->insert(pair);//���뵽�Լ��Ļ�����
            }
            friend inline std::ostream& operator<<(std::ostream& os, const CachedFunction& func)noexcept {//���������
                AUTOLOG//�Զ���¼��־
                return  os<<func.m_Cache->size() << std::endl;//�������Ĵ�С��������
            }
            inline std::function<FuncType>* operator->()noexcept { AUTOLOG return &m_Func; }//���غ��������ָ��
            inline bool empty()const noexcept { AUTOLOG return m_Cache->empty(); }//�жϻ����Ƿ�Ϊ��
            inline std::size_t size()const noexcept { AUTOLOG return m_Cache->size(); }//���ػ���Ĵ�С
            SAFE_BUFFER inline iterator GetRef(const Args&...args)const noexcept {//�����±������
                AUTOLOG//�Զ���¼��־
                auto&& key = std::forward_as_tuple(std::cref(args)...);
                return  m_Cache->find(key);//�������ݵ��ǲ�����Ƿ����
            }
        };
        template<typename R> struct CachedFunction<R> : public CachedFunctionBase {//û�в����Ļ��溯����һ���̳���CachedFunctionBase����
            mutable std::function<R()> m_Func;
            mutable CacheitemType<R> m_Cache;
            inline CachedFunction(const CacheitemType<R>& cache) :m_Func(nullptr), m_Cache(cache) { AUTOLOG }
            inline CachedFunction(void* funcAddr, CallType type, const std::function<R()>& func, TimeType cacheTime = MEMOIZATIONSEARCH) : CachedFunctionBase(funcAddr, type, cacheTime), m_Func(std::move(func)), m_Cache(std::make_pair(R{}, 0)) { AUTOLOG }
            inline R& setcacheresettime(const R& value, const  TimeType  cacheTime = ApproximatelyGetCurrentTime())const noexcept {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);
                m_Cache = std::make_pair(value, cacheTime);
                return m_Cache.first;
            }
            inline std::string GetObjectName() const noexcept {//��ȡ��������� ���͵�����
                AUTOLOG//�Զ���¼��־
                return typeid(CachedFunction).name();
            }
            inline operator bool()noexcept { AUTOLOG  return (bool)m_Func; }//��麯���Ƿ���Ч
            inline operator std::function<R()>()noexcept { AUTOLOG  return m_Func; }
             inline CachedFunction(CachedFunction&& other) noexcept : CachedFunctionBase(other.m_funcAddr, other.m_callType, other.m_cacheTime), m_Func(std::move(other.m_Func)), m_Cache(std::move(other.m_Cache)) {
                AUTOLOG//�Զ���¼��־
                m_funcAddr = std::move(other.m_funcAddr);
            }
            inline CachedFunction& operator=(CachedFunction&& others) noexcept {
                AUTOLOG//�Զ���¼��־
                m_Func = std::move(others.m_Func);
                m_Cache = std::move(others.m_Cache);
                m_funcAddr = others.m_funcAddr;
                others.m_funcAddr = nullptr;
                return *this;
            }
            inline CachedFunction(const CachedFunction& others) :CachedFunctionBase(others.m_funcAddr, others.m_callType, others.m_cacheTime), m_Cache(others.m_Cache), m_Func(others.m_Func) { AUTOLOG }
            inline CachedFunction& operator=(const CachedFunction& others)noexcept {
                AUTOLOG//�Զ���¼��־
                m_Cache = others.m_Cache;
                m_Func = others.m_Func;
                m_funcAddr = others.m_funcAddr;
                return *this;
            }
            inline bool LoadCache(const char* szFileName)noexcept {
                AUTOLOG//�Զ���¼��־
                std::ifstream file(GetUniqueName(m_Func,szFileName), std::ios_base::binary);
                if (!file.is_open() || !operator<<(file)) return false;
                file.close();
                return true;
            }
            inline bool SaveCache(const char* szFileName) noexcept {
                AUTOLOG//�Զ���¼��־
                std::ofstream file(GetUniqueName(m_Func,szFileName), std::ios::binary |std::ios::out|std::ios::trunc);
                if (!file.is_open() || !operator>>(file)) return false;
                file.close();
                return true;
            }
            inline bool operator >> (const std::ofstream& file) noexcept {
                AUTOLOG//�Զ���¼��־
                if (!file.is_open()) return false;
                if (!WritePairToFile(file, m_Cache))return false;
                return true;
            }
            inline bool operator << (const std::ifstream& file) noexcept {
                AUTOLOG//�Զ���¼��־
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
                AUTOLOGPERFIX(typeid(CachedFunction).name() + std::string(xorstr("Functor")))//�Զ���¼��־
                if ((TimeType)m_Cache.second > (TimeType)ApproximatelyGetCurrentTime()) return m_Cache.first;
                return setcacheresettime(m_Func(),(safeadd(ApproximatelyGetCurrentTime(), m_cacheTime)));
            }
            inline void CleanCache()const noexcept {
                AUTOLOG//�Զ���¼��־
                ScopeLock lock(m_mutex);//����
                m_Cache.second = ApproximatelyGetCurrentTime() - 1, m_Cache.first = R(); 
            }
             inline std::function<R()>* operator->()noexcept { AUTOLOG  return &m_Func; }
            inline bool empty()const noexcept { 
                AUTOLOG//�Զ���¼��־
                return std::hash(m_Cache.first) == std::hash(R{}); //��������ֵ��Ĭ��ֵ��Ϊ�� 
            }
            inline std::size_t size()const noexcept { 
                AUTOLOG//�Զ���¼��־
                return 1; //����ֻ��һ������
            }
            inline CacheitemType<R>& GetRef() {
                return m_Cache;
            }
        };
        //��ȡ�����ķ���ֵ�Ͳ���
        template <typename F>struct function_traits : function_traits<decltype(&F::operator())> {};//��F������һ��lambda���ʽ��ʱ
        template <typename R, typename... Args>struct function_traits<R(*)(Args...)> {//����ָ�����ȡ�����пɱ����
            using return_type = R;
            using args_tuple_type = std::tuple<Args...>;
        };
        template <typename R>struct function_traits<R(*)()> {//����ָ�����ȡ��û�в���
            using return_type = R;
            using args_tuple_type = std::tuple<>;
        };
        template <typename R, typename... Args>struct function_traits<std::function<R(Args...)>> {//std::function����ȡ�����пɱ����
            using return_type = R;
            using args_tuple_type = std::tuple<Args...>;
        };
        template <typename R>struct function_traits<std::function<R()>> {//std::function����ȡ��û�в���
            using return_type = R;
            using args_tuple_type = std::tuple<>;
        };
        template <typename ClassType, typename R, typename... Args>
        struct function_traits<R(ClassType::*)(Args...) const> {//��Ա��������ȡ�����пɱ����
            using return_type = R;
            using args_tuple_type = std::tuple<Args...>;
        };
        template <typename ClassType, typename R>
        struct function_traits<R(ClassType::*)() const> {//��Ա��������ȡ��û�в���
            using return_type = R;
            using args_tuple_type = std::tuple<>;
        };
        template<typename ClassType, typename R, typename... Args>
        struct function_traits<R(ClassType::*)(Args...)> : function_traits<R(Args...)> {};//��Ա��������ȡ�����пɱ����
#ifdef _WIN64
        template <typename F>struct stdcallfunction_traits : stdcallfunction_traits<decltype(&F::operator())> {};//stdcall��lambda���ʽ��ȡ
        template <typename R, typename... Args>struct stdcallfunction_traits<R(STDCALL*)(Args...)> :public function_traits<R(STDCALL*)(Args...)> {};//stdcall�ĺ���ָ����ȡ�����пɱ����
        template <typename R>struct stdcallfunction_traits<R(STDCALL*)()> :public function_traits<R(STDCALL*)()> {};//stdcall�ĺ���ָ����ȡ��û�в���
        template <typename ClassType, typename R, typename... Args>struct stdcallfunction_traits<R(STDCALL ClassType::*)(Args...)> :public function_traits<R(STDCALL ClassType::*)(Args...)> {};//stdcall�ĳ�Ա������ȡ�����пɱ����
        template <typename ClassType, typename R>struct stdcallfunction_traits<R(STDCALL ClassType::*)()> :public function_traits<R(STDCALL ClassType::*)()> {};//stdcall�ĳ�Ա������ȡ��û�в���
        template <typename ClassType, typename R, typename... Args>struct stdcallfunction_traits<R(STDCALL ClassType::*)(Args...) const> :public function_traits<R(STDCALL ClassType::*)(Args...) const > {};//stdcall�ĳ�Ա������ȡ�����пɱ����
        template <typename ClassType, typename R>struct stdcallfunction_traits<R(STDCALL ClassType::*)() const> :public function_traits<R(STDCALL ClassType::*)() const > {};//stdcall�ĳ�Ա������ȡ��û�в���
        template <typename F>struct fastcallfunction_traits : fastcallfunction_traits<decltype(&F::operator())> {};//fastcall��lambda���ʽ��ȡ
        template <typename R, typename... Args>struct fastcallfunction_traits<R(FASTCALL*)(Args...)> :public function_traits<R(FASTCALL*)(Args...)> {};//fastcall�ĺ���ָ����ȡ�����пɱ����
        template <typename R>struct fastcallfunction_traits<R(FASTCALL*)()> :public function_traits<R(FASTCALL*)()> {};//fastcall�ĺ���ָ����ȡ��û�в���
        template <typename ClassType, typename R, typename... Args>struct fastcallfunction_traits<R(FASTCALL ClassType::*)(Args...)> :public function_traits<R(FASTCALL ClassType::*)(Args...)> {};//fastcall�ĳ�Ա������ȡ�����пɱ����
        template <typename ClassType, typename R>struct fastcallfunction_traits<R(FASTCALL ClassType::*)()> :public function_traits<R(FASTCALL ClassType::*)()> {};//fastcall�ĳ�Ա������ȡ��û�в���
        template <typename ClassType, typename R, typename... Args>struct fastcallfunction_traits<R(FASTCALL ClassType::*)(Args...) const> :public function_traits<R(FASTCALL ClassType::*)(Args...) const> {};//fastcall�ĳ�Ա������ȡ�����пɱ����
        template <typename ClassType, typename R>struct fastcallfunction_traits<R(FASTCALL ClassType::*)() const> :public function_traits <R(FASTCALL ClassType::*)() const> {};//fastcall�ĳ�Ա������ȡ��û�в���
#endif // _WIN64
        template <typename R, typename... Args> inline static auto& GetCachedFunction(void* funcAddr, CallType type, const std::function<R(Args...)>& func, TimeType cacheTime = MEMOIZATIONSEARCH) noexcept {
            AUTOLOGPERFIX(typeid(std::function<R(Args...)>).name())//�Զ���¼��־
            return  ObjectPool<CachedFunction<R, Args...>>::Construct()(cacheTime, funcAddr, type, func, cacheTime);//���ݲ��������͹���һ�����溯��
        }
        template <typename R> inline static auto GetCachedFunction(void* funcAddr, CallType type, const std::function<R()>& func, TimeType cacheTime = MEMOIZATIONSEARCH) noexcept {
            AUTOLOGPERFIX(typeid(std::function<R()>).name())//�Զ���¼��־
            return  CachedFunction<R>(funcAddr, type, func, cacheTime);//���ݲ��������͹���һ�����溯��
        }
        template<typename F, std::size_t... Is, typename DecayF = std::decay_t<F>> inline static auto MakeCachedImpl(F&& f, CallType type, TimeType validtime, const std::index_sequence<Is...>&) noexcept {
            AUTOLOGPERFIX(typeid(F).name())//�Զ���¼��־
            using Functype = typename function_traits<DecayF>::return_type(typename std::tuple_element<Is, typename function_traits<DecayF>::args_tuple_type>::type...);
            return GetCachedFunction((void*)&f, type, std::function<Functype>(std::forward<F>(f)), validtime);//���ݲ��������͹���һ�����溯��
        }
        template<typename T, typename R, typename... Args>SAFE_BUFFER inline auto ThisCall(T& obj, R(T::* func)(Args...))noexcept { 
            AUTOLOGPERFIX(typeid(T).name())//�Զ���¼��־
            return [&obj, func](const Args&... args) -> R {return (obj.*func)(args...); }; //��װT���͵ĳ�Ա������Ϊһ���ɵ��õĺ�������
        }
        template<typename T, typename F>inline auto ConvertMemberFunction(const T& obj, F&& func)noexcept { AUTOLOG return ThisCall(const_cast<T&>(obj), std::forward<F>(func)); }//ת����Ա������Ϊһ���ɵ��õĺ�������
#ifdef _MSC_VER
#pragma  warning(  pop  )
#endif
    }
    template<typename T> using ArgsType_v = std::make_index_sequence<std::tuple_size<T>::value>;//ͨ��tuple�Ĵ�С����һ��index_sequence
}
//���溯���Ĵ����ļ�ǿ��
template<typename F>inline  auto MakeCacheEx(F&& f, const  memoizationsearch::nonstd::CallType& calltype= memoizationsearch::nonstd::CallType::cdeclcall, TimeType _validtime=MEMOIZATIONSEARCH)noexcept {
    AUTOLOGPERFIX(typeid(F).name())
    using namespace memoizationsearch;
	using namespace memoizationsearch::nonstd;
    auto validtime =Clamp(_validtime, (TimeType)1, INFINITYCACHE);//��Чʱ�� ��Ӳ������ ���ֻ�᷵��INFINITYCACHE
#ifdef _WIN64
    switch (calltype) {//���ݲ�ͬ�ĵ��÷�ʽ������ͬ�Ļ��溯��
    case  CallType::cdeclcall:break;
        //����stdcall�����⴦��
    case  CallType::stdcall:  return  MakeCachedImpl(f, calltype, validtime, ArgsType_v<typename  stdcallfunction_traits<std::decay_t<F>>::args_tuple_type>{});
        //����fastcall�����⴦��
    case  CallType::fastcall: return  MakeCachedImpl(f, calltype, validtime,ArgsType_v<typename  fastcallfunction_traits<std::decay_t<F>>::args_tuple_type>{});
    }
#endif // _WIN64
    return  MakeCachedImpl(f, calltype, validtime,ArgsType_v<typename function_traits<std::decay_t<F>>::args_tuple_type>{});//���ݲ��������͹���һ�����溯��
}
template<typename R, typename...Args>  inline auto& MakeCacheItem(const R& ret, TimeType validtime, const Args&...args) noexcept { 
    AUTOLOG//�Զ���¼��־
    return std::make_pair(std::make_tuple(args...), std::make_pair(ret, validtime)); 
}
template<typename F> inline auto MakeCache(F&& f, const TimeType validtime = MEMOIZATIONSEARCH) noexcept {//�������� �ڶ��������ǻ������Чʱ�� Ĭ����MEMOIZATIONSEARCH
    AUTOLOGPERFIX(typeid(F).name())//�Զ���¼��־
    return MakeCacheEx(f, memoizationsearch::nonstd::CallType::cdeclcall, validtime);//��Ĭ�ϲ����ĺ���д�����Ϊ�˰�validtime���ݸ���
}
template<typename T, class F> inline auto CacheMemberFunction(T&& obj, F&& func,const TimeType validtime = MEMOIZATIONSEARCH) noexcept { //�����Ա����
    AUTOLOG//�Զ���¼��־
    return MakeCacheEx(memoizationsearch::nonstd::ConvertMemberFunction(std::forward<T>(obj), std::forward<F>(func)), memoizationsearch::nonstd::CallType::cdeclcall, validtime);//��ת��Ϊһ��ĺ����ڵ���,��Ĭ�ϲ����ĺ���д�����Ϊ�˰�validtime���ݸ���
}
#endif

