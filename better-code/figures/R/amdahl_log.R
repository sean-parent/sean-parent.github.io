library(ggplot2)
library(scales)

amdahl <- function(x, p) {1.0 / ((1.0 - p) + p / x)}

amdahlFun05 <- function(x) amdahl(x, 0.95)
amdahlFun10 <- function(x) amdahl(x, 0.90)
amdahlFun25 <- function(x) amdahl(x, 0.75)
amdahlFun50 <- function(x) amdahl(x, 0.50)

LegendTitle = "Percentage of\nsynchronization"

p1 <- ggplot(data = data.frame(x = c(1, 65536)), mapping = aes(x=x)) + 
    stat_function(fun = amdahlFun05, mapping = aes(colour = "amdahlFun05", linetype = "amdahlFun05")) +
    stat_function(fun = amdahlFun10, mapping = aes(colour = "amdahlFun10", linetype = "amdahlFun10")) +
    stat_function(fun = amdahlFun25, mapping = aes(colour = "amdahlFun25", linetype = "amdahlFun25")) +
    stat_function(fun = amdahlFun50, mapping = aes(colour = "amdahlFun50", linetype = "amdahlFun50")) +
    scale_x_continuous(trans = 'log2', name = "Number of Processors", limits=c(1, 65536), 
        breaks = trans_breaks("log2", n = 16, function(x) 2^x), labels = trans_format("log2", function(x) 2^x)) +
    scale_y_continuous(name = "Speedup", breaks = seq(0, 21, 2), limits=c(1, 20)) +
    scale_colour_manual(LegendTitle, 
        values = c("amdahlFun05" = "red", "amdahlFun10" = "orange", "amdahlFun25" = "deeppink", "amdahlFun50" = "blue"),
        labels = c("5%", "10%", "25%", "50%")) +
    scale_linetype_manual(LegendTitle, 
        values = c("amdahlFun05" = "solid", "amdahlFun10" = "dashed", "amdahlFun25" = "longdash", "amdahlFun50" = "dotted"),
        labels = c("5%", "10%", "25%", "50%")) + 
    theme_bw() +
    theme(axis.line = element_line(size=1, colour = "black"),
              panel.grid.major = element_line(colour = "#d3d3d3"),
              panel.grid.minor = element_blank(),
              panel.border = element_blank(), panel.background = element_blank(),
              text=element_text(family="Arial"),
              legend.title=element_text(size = 10, family = "Arial"),
              legend.text = element_text(size = 10),
              legend.position = c(0.25, 0.8),
              axis.text.x=element_text(colour="black", size = 8),
              axis.text.y=element_text(colour="black", size = 8),
              axis.title.x=element_text(colour="black", size = 12),
              axis.title.y=element_text(colour="black", size = 12))
              
 ggsave(file="../05-amdahl_log.svg", plot=p1, width=8, height=6)


