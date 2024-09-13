using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;

public class Memoizer<TResult> where TResult : new()  // This constraint ensures TResult has a parameterless constructor
{
    private readonly Func<object[], TResult> func;
    private readonly Dictionary<Tuple<object[]>, (TResult Result, ulong Timestamp)> cache;
    private ulong timeout;

    public Memoizer(Func<object[], TResult> function, ulong timeout)
    {
        this.func = function;
        this.cache = new Dictionary<Tuple<object[]>, (TResult Result, ulong Timestamp)>(new TupleComparer());
        this.timeout = timeout;
    }

    public TResult Call(params object[] args)
    {
        ulong currentMillis = (ulong)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
        var key = Tuple.Create(args);

        if (cache.ContainsKey(key))
        {
            (TResult result, ulong timestamp) = cache[key];
            if (currentMillis - timestamp < timeout || timeout == ulong.MaxValue)
                return result;
        }

        TResult newResult = func(args);
        cache[key] = (newResult, currentMillis);
        return newResult;
    }
    public TResult Seek(params object[] args)
    {
        // Create a key from the arguments array
        var key = Tuple.Create(args);

        // Check if the key exists in the cache
        if (cache.ContainsKey(key))
        {
            // Retrieve the cached result
            (TResult result, ulong timestamp) = cache[key];
            // Optionally, update timestamp or add logic to handle stale data
            return result;
        }

        // Return a new instance of TResult if not found in cache
        return new TResult();
    }
    private class TupleComparer : IEqualityComparer<Tuple<object[]>>
    {
        public bool Equals(Tuple<object[]> x, Tuple<object[]> y)
        {
            return x.Item1.SequenceEqual(y.Item1);
        }

        public int GetHashCode(Tuple<object[]> obj)
        {
            unchecked // Overflow is fine, just wrap
            {
                int hash = 19;
                foreach (var item in obj.Item1)
                {
                    hash = hash * 31 + (item?.GetHashCode() ?? 0);
                }
                return hash;
            }
        }
    }
    public void ClearCache(params object[] args)
    {
        var key = Tuple.Create(args);
        if (cache.ContainsKey(key))
        {
            cache.Remove(key);
        }
    }


    public void SaveToFile(string filename)
    {
        using (var stream = File.OpenWrite(filename))
        {
            var formatter = new BinaryFormatter();
            formatter.Serialize(stream, cache);
        }
    }

    public void LoadFromFile(string filename)
    {

            using (var stream = File.OpenRead(filename))
            {
                var formatter = new BinaryFormatter();
                var deserializedData = (Dictionary<Tuple<object[]>, (TResult Result, ulong Timestamp)>)formatter.Deserialize(stream);
                cache.Clear();
                foreach (var item in deserializedData)
                {
                    cache.Add(item.Key, item.Value);
                }
            }
    }


    public void SetCacheTimeout(ulong newTimeout)
    {
        timeout = newTimeout;
    }

    public ulong GetCacheTimeout()
    {
        return timeout;
    }

    public int CacheSize()
    {
        return cache.Count;
    }

    public override string ToString()
    {
        return $"Cache contains {CacheSize()} items.";
    }
}
