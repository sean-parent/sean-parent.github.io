library(ggplot2)
library(scales)

amdahlFun95 <- function(x) {
    1.0 / ((1.0 - 0.95) + 0.95 / x)
}
amdahlFun90 <- function(x) {
    1.0 / ((1.0 - 0.9) + 0.9 / x)
}
amdahlFun75 <- function(x) {
    1.0 / ((1.0 - 0.75) + 0.75 / x)
}
amdahlFun50 <- function(x) {
    1.0 / ((1.0 - 0.50) + 0.50 / x)
}

p1 <- ggplot(data.frame(x = c(1, 65536)), aes(x=x)) + 
    stat_function(fun = amdahlFun95, aes(colour = "95%")) +
    stat_function(fun = amdahlFun90, aes(colour = "90%")) +
    stat_function(fun = amdahlFun75, aes(colour = "75%")) +
    stat_function(fun = amdahlFun50, aes(colour = "50%")) +
    scale_x_continuous(trans = 'log2', name = "Number of Processors", limits=c(1, 65536), 
        breaks = trans_breaks("log2", n = 16, function(x) 2^x), labels = trans_format("log2", function(x) 2^x)) +
    scale_y_continuous(name = "Speedup", breaks = seq(0, 21, 2), limits=c(1, 20)) +
        scale_colour_manual("Parallel Portion", 
        limits=c("95%", "90%", "75%", "50%"), 
        values = c("95%" = "red", "90%" = "orange", "75%" = "deeppink", "50%" = "blue")) +
    theme_bw()
    
 ggsave(file="05_amdahl_log.svg", plot=p1, width=12, height=10)


