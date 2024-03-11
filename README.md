## 基于跳表的KV存储引擎

> 项目使用xmake构建，请确保您已安装xmake
> xmake官网：https://xmake.io/#/zh-cn/

### 构建与运行

```sh
xmake build skiplist

# or ./bin/skiplist 
xmake run skiplist
```

### 压力测试

```bash
chmod +x ./run_test.sh
./run_test.sh
```