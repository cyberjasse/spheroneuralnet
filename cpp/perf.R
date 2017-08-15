path = "~/Bureau/spheroneuralnet/cpp/tests/perf/perf"
f = 10
filename = paste(path,f, sep="")
filenameextension = paste(filename, ".txt", sep="")
table = read.table(file = filenameextension ,header = T)
data = table[,1]
#tout mettre à partir de 0
data = data - min(data)
#creer temps cumule d'un timer a frequence f
T = 1000000*(1/f)
timer = seq(0, (length(data)-1)*T, T)
#soustraire
data1 = data - timer
filenameextension = paste(filename, "Before.png", sep="")
png(filename = filenameextension)
title = paste(f,"Hz streaming - ",f,"Hz clock", sep="")
plot(data1, type="l", xlab = "Message", ylab = "Reception date - expected date (µs)", main = title)
dev.off()

#maintenant on corrige la fréquence et on refait
ra = seq(0,length(data1)-1,1)
data1.lm = lm(data1 ~ ra)
slope = coef(data1.lm)["ra"]
T = T+slope
newf = 1/(T/1000000)
newf

timer = seq(0, (length(data)-1)*T, T)
data2 = data - timer
filenameextension = paste(filename, "After.png", sep="")
png(filename = filenameextension)
newfr = round(newf, digits = 4)
title = paste(f,"Hz streaming - ",newfr,"Hz clock", sep="")
plot(data2, type="l", xlab = "Message", ylab = "Reception date - expected date (µs)", main = title)
dev.off()
