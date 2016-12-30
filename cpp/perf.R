table = read.table(file = "~/Bureau/spheroneuralnet/cpp/perfTop8.txt")
usi = 3
si = usi-1
correct = table[table[usi]<0, usi] + 1000000
data = table[table[usi] > 150, usi]
data = c(data,correct)
data.mean = mean(data)
data.sd = sd(data)
data.mean
data.sd
ksresult = ks.test(data,"pnorm",mean=data.mean,sd=data.sd)
ksresult

hist(data,breaks=1000,xlim=c(min(data),30000),xlab="packet reception time (µs)",main="Histogram streaming ??Hz")
