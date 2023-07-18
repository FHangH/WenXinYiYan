# WenXinYiYan
百度文心一言-UE插件

# 大致说明
使用UE C++，基于UE Http模块，没有第三方库，理论支持所有版本的UE，和所有平台，可以自行编译，用于对应的UE版本

# 注
- 随便做着玩的，目前支持流式响应
- 但文心一言好像本身不支持像ChatGPT那样详细的定制，本身很粗糙，应该不支持上下文联系（毕竟官方的API文档里的多轮请求和响应结果都是有问题的）

# 蓝图节点

注意这是默认的第一步，必须要有的，至于拿到的token怎么用无所谓，后面的节点里，已经默认使用生成的token了
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/0d7d633e-1531-4965-a8b3-8ecb13135b74)

文心一言的role里，只有user和assistant，不像GPT有system，官方的prompt看着难受，看不懂，懒得用了，只能这样了
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/31eddf5b-b038-45a9-a7c6-b3460be08f5f)
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/e218a4bc-3d19-4882-a58d-3ec16c0d41ec)

流式响应很粗糙，我是说文心一言，不像GPT那样细分，没那么丝滑
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/d730186b-5a64-48c8-a23b-bebb07c07497)

```log
LogWenXin: Warning: ====== WenXinYiYan Start ======
LogWenXin: Warning: Request Stream Process
LogWenXin: Warning: Request Stream Complete
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = C++中的单例模式可以分为懒汉式和饿汉式两种实现方式。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = 懒汉式单例模式在第一次使用时才创建对象，而饿汉式单例模式则在程序启动时就创建对象。同时，为了线程安全，还需要考虑单例对象的线程安全问题。
1. 懒汉式单例模式
懒汉式单例模式是一种延迟加载的方式，在第一次使用时才创建对象。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = 可以通过以下代码实现：

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

在这个实现中，getInstance()函数是一个静态函数，用于获取单例对象。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = getInstance()函数内部使用静态局部变量instance来存储单例对象，保证在多线程环境下只有一个实例存在。
2. 饿汉式单例模式
饿汉式单例模式是一种在程序启动时就创建对象的方式。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = 可以通过以下代码实现：

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

在这个实现中，getInstance()函数是一个静态函数，用于获取单例对象。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = getInstance()函数内部使用静态局部变量instance来存储单例对象，保证在多线程环境下只有一个实例存在。
3. 线程安全
在多线程环境下，为了保证单例对象的线程安全，需要采取一些措施。
* 静态局部变量：在getInstance()函数内部使用静态局部变量来存储单例对象，这样在多线程环境下只有一个实例存在。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = 
* 线程安全的数据结构：可以使用互斥锁、信号量等线程安全的数据结构来保证单例对象的线程安全。
* 静态成员变量初始化：在静态局部变量instance的初始化中，可以加上静态初始化语法，这样在多线程环境下也可以保证只有一个实例存在。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = 
例如：

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton instance;
};
Singleton Singleton::instance;
int main() {
    // 创建多个线程
    std::thread t1([](){
        Singleton& instance = Singleton::getInstance();
    });
    std::thread t2([](){
        Singleton& instance = Singleton::getInstance();
    });
    t1.join();
    t2.join();
    return 0;
}

在这个实现中，getInstance()函数内部使用静态局部变量instance来存储单例对象，这样在多线程环境下只有一个实例存在。
LogWenXin: Warning: Request Process By Stream
LogWenXin: Warning: Deserialize Stream Response
LogWenXin: Warning: Stream Result = 同时，在静态局部变量instance的初始化中，使用了静态初始化语法，这样可以保证在多线程环境下也可以只有一个实例存在。
LogWenXin: Warning: Response Stream Success
LogWenXin: Warning: Request Stream Result = assistant : C++中的单例模式可以分为懒汉式和饿汉式两种实现方式。懒汉式单例模式在第一次使用时才创建对象，而饿汉式单例模式则在程序启动时就创建对象。同时，为了线程安全，还需要考虑单例对象的线程安全问题。
1. 懒汉式单例模式
懒汉式单例模式是一种延迟加载的方式，在第一次使用时才创建对象。可以通过以下代码实现：

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

在这个实现中，getInstance()函数是一个静态函数，用于获取单例对象。getInstance()函数内部使用静态局部变量instance来存储单例对象，保证在多线程环境下只有一个实例存在。
2. 饿汉式单例模式
饿汉式单例模式是一种在程序启动时就创建对象的方式。可以通过以下代码实现：

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

在这个实现中，getInstance()函数是一个静态函数，用于获取单例对象。getInstance()函数内部使用静态局部变量instance来存储单例对象，保证在多线程环境下只有一个实例存在。
3. 线程安全
在多线程环境下，为了保证单例对象的线程安全，需要采取一些措施。
* 静态局部变量：在getInstance()函数内部使用静态局部变量来存储单例对象，这样在多线程环境下只有一个实例存在。
* 线程安全的数据结构：可以使用互斥锁、信号量等线程安全的数据结构来保证单例对象的线程安全。
* 静态成员变量初始化：在静态局部变量instance的初始化中，可以加上静态初始化语法，这样在多线程环境下也可以保证只有一个实例存在。
例如：

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton instance;
};
Singleton Singleton::instance;
int main() {
    // 创建多个线程
    std::thread t1([](){
        Singleton& instance = Singleton::getInstance();
    });
    std::thread t2([](){
        Singleton& instance = Singleton::getInstance();
    });
    t1.join();
    t2.join();
    return 0;
}

在这个实现中，getInstance()函数内部使用静态局部变量instance来存储单例对象，这样在多线程环境下只有一个实例存在。同时，在静态局部变量instance的初始化中，使用了静态初始化语法，这样可以保证在多线程环境下也可以只有一个实例存在。
LogWenXin: Warning: ====== WenXinYiYan End ======
```

这个我承认我自己写的糙了，我不会写

![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/7b9d9c2d-3b4f-4deb-8385-94bc0c4badf7)

这里演示的是流式响应过程中，中断流式响应的结果，不会调用OnFail，回调OnCompleted，回调结果默认直接为空
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/10c411ee-97ba-4159-b5a6-0cbd3e1a595c)
