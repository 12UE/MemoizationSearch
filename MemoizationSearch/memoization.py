import pickle
import time
import random
cachetime=200
INFINITYCACHE=0xffffffff
class Memoizer:
    def __init__(self, func, timeout=cachetime):
        self.func = func
        self.cache = {}
        self.timeout = timeout
    def __call__(self, *args):
        current_time = time.time_ns() // 1_000_000 #ms
        if args in self.cache:
            result, timestamp = self.cache[args]
            if current_time - timestamp < self.timeout:
                return result
        result = self.func(*args)
        self.cache[args] = (result, current_time)
        return result
    def ClearCache(self):
        self.cache.clear()
    def ClearCache(self,*args):
        del self.cache[args]
    def getrandomNumbers(self):
        return random.randint(0, 100)
    def save_to_file(self, filename):
        with open(filename, 'wb') as f:
            pickle.dump(self.cache, f)
        print(f"Cache saved to {filename}")
    def load_from_file(self, filename):
        try:
            with open(filename, 'rb') as f:
                self.cache = pickle.load(f)
            print(f"Cache loaded from {filename}")
        except FileNotFoundError:
            print(f"No cache file found at {filename}")
    def __rshift__(self, file):
         pickle.dump(self.cache, file)
    def __lshift__(self, data):
        self.cache = pickle.load(data)
    def seek(self,*args):
        if args in self.cache:
            result, timestamp = self.cache[ args]
            return result
        return None;
    def SetCacheResetTime(self,retvalue,validtime,*args):
        self.cache[args] = {retvalue,validtime}
    def SetCacheTime(self,time):
        self.timeout=time;
    def GetCacheTime(self):
        return self.timeout
    def __iter__(self):
        return iter(self.cache.items())
    def __str__(self):
        return f"Cache contains {self.size()} items."
    def empty(self):
        return self.size()
    def size(self):
        return len(self.cache)

