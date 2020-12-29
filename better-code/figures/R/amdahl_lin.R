library(ggplot2)
library(scales)

amdahl <- function(x, p) {1.0 / ((1.0 - p) + p / x)}

amdahlFun10 <- function(x) amdahl(x, 0.90)
amdahlFun20 <- function(x) amdahl(x, 0.80)
amdahlFun30 <- function(x) amdahl(x, 0.70)
amdahlFun40 <- function(x) amdahl(x, 0.60)
optimalFun <- function(x) x

LegendTitle = "Percentage of\nsynchronization"

p1 <- ggplot(data.frame(x = c(1, 16)), aes(x=x)) + 
    stat_function(fun = optimalFun,  aes(colour = "amdahlFun00", linetype="amdahlFun00")) +
    stat_function(fun = amdahlFun10, aes(colour = "amdahlFun10", linetype="amdahlFun10")) +
    stat_function(fun = amdahlFun20, aes(colour = "amdahlFun20", linetype="amdahlFun20")) +
    stat_function(fun = amdahlFun30, aes(colour = "amdahlFun30", linetype="amdahlFun30")) +
    stat_function(fun = amdahlFun40, aes(colour = "amdahlFun40", linetype="amdahlFun40")) +
    scale_x_continuous(name = "Number of Processors", breaks = seq(1, 16, 1), limits=c(1, 16)) +
    scale_y_continuous(name = "Speedup", breaks = seq(1, 16, 1)) +
    scale_colour_manual(LegendTitle, 
        labels = c("0%", "10%", "20%", "30%", "40%"), 
        values = c("amdahlFun00" = "green", "amdahlFun10" = "red", "amdahlFun20" = "orange", "amdahlFun30" = "deeppink", "amdahlFun40" = "blue")) +
    scale_linetype_manual(LegendTitle, 
        values = c("amdahlFun00" = "solid", "amdahlFun10" = "dashed", "amdahlFun20" = "longdash", "amdahlFun30" = "dotted", "amdahlFun40" = "dotdash"),
        labels = c("0%", "10%", "20%", "30%", "40%")) +
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
    
ggsave(file="../05-amdahl_lin.svg", plot=p1, width=8, height=8)


