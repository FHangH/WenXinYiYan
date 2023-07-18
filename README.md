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

流式响应很粗糙，我是说文心一言，不像GPT那样细分，没那么丝滑
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/d730186b-5a64-48c8-a23b-bebb07c07497)

这个我承认我自己写的糙了，我不会写
![image](https://github.com/FHangH/WenXinYiYan/assets/49579735/7b9d9c2d-3b4f-4deb-8385-94bc0c4badf7)
