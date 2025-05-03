# ReactorHTTP Server

## 简介

待完善


### 环境要求

* Linux 系统
* G++ 编译器
* Make 工具


### 安装步骤

1. 克隆本仓库

```bash
git clone https://github.com/your-username/ReactHTTP.git
cd ReactorHTTP
```

2. 编译并安装

```bash
make
sudo make install
```

3. 运行服务器

```bash
./server -p 55655
```

此时服务器将启动并监听 `55655` 端口。

## 配置

服务器支持以下配置项：

* **端口号**（`-p`）：指定服务器监听的端口（默认：55655）
* **工作路径**（`-d`）：设置服务器的工作路径，默认./root
* **日志文件**（`-l`）：设置日志文件路径（默认：`/var/log/reactorhttp.log`）

例如，启动一个监听在 `8080` 端口的服务器，最大并发数为 2000：

```bash
./server -p 8080 -d ./root
```

## 性能测试

可以使用工具如 [wrk](https://github.com/wg/wrk) 或 [Locust](https://locust.io/) 来测试服务器的性能。例如：

```bash
wrk -c 20000 -d 30s http://127.0.0.1:55655/
```

这会使用 `20000` 个并发连接进行 30 秒的负载测试。
### 测试结果
* **并发连接数**：20,000
* **持续时间**：30秒
* **请求成功数**：3,799,918 请求
* **请求速率**：126,136 请求/秒
* **数据传输速率**：103.81MB/秒
* **平均延迟**：66.24ms
* **最大延迟**：166.23ms
* **标准差**：9.11ms


## 贡献

欢迎提交 Issues 和 Pull Requests，改进这个项目或添加新特性！


