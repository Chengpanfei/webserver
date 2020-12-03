# 一个简单的web静态服务器

借鉴了Netty的ChannelHandler的思想。
EventPoll 接收数据 -> inHandler1 -> inHandler2
发送数据 outHandler2 -> outHandler1 -> EventPoll