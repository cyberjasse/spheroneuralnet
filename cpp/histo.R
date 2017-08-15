path = "~/Bureau/spheroneuralnet/cpp/tests/perf/period/perf"
fs = c(100,200)
for(f in fs){
filename = paste(path,f, sep="")
filenameextension = paste(filename, "period.txt", sep="")
table = read.table(file = filenameextension ,header = T)
data = table[,1]

filenameextension = paste(filename, ".png", sep="")
png(filename = filenameextension)
title = paste("Histogram of a ",f,"Hz streaming", sep="")
hist(data,breaks=max(data)/1000 ,xlim=c(0,450000),ylim=c(0,130),xlab = "Reception time (µs)", main=title)
dev.off()
}