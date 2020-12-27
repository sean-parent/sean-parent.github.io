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

optimalFun <- function(x) {
    x
}

p1 <- ggplot(data.frame(x = c(1, 16)), aes(x=x)) + 
    stat_function(fun = optimalFun, aes(colour = "100%")) +
    stat_function(fun = amdahlFun95, aes(colour = "95%")) +
    stat_function(fun = amdahlFun90, aes(colour = "90%")) +
    stat_function(fun = amdahlFun75, aes(colour = "75%")) +
    stat_function(fun = amdahlFun50, aes(colour = "50%")) +
    scale_x_continuous(name = "Number of Processors", breaks = seq(1, 16, 1), limits=c(1, 16)) +
    scale_y_continuous(name = "Speedup", breaks = seq(1, 16, 1)) +
    scale_colour_manual("Parallel Portion", 
        limits=c("100%", "95%", "90%", "75%", "50%"), 
        values = c("100%" = "green", "95%" = "red", "90%" = "orange", "75%" = "deeppink", "50%" = "blue")) +
    theme_bw() +
    theme(axis.line = element_line(size=1, colour = "black"),
              panel.grid.major = element_line(colour = "#d3d3d3"),
              panel.grid.minor = element_blank(),
              panel.border = element_blank(), panel.background = element_blank(),
              plot.title = element_text(size = 14, family = "Arial", face = "bold"),
              text=element_text(family="Arial"),
              axis.text.x=element_text(colour="black", size = 9),
              axis.text.y=element_text(colour="black", size = 9))
    
ggsave(file="05_amdahl_lin.svg", plot=p1, width=10, height=10)


