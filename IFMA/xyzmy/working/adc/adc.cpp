#include "adc.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#include "ui_adc.h"

ADC::ADC(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ADC)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Test()));
}

ADC::~ADC()
{
    delete ui;
}



void ADC::Test()
{
        fprintf(stderr, "press Ctrl-C to stop\n");
        int fd = open("/dev/adc", 0);
        if (fd < 0) {
                perror("open ADC device:");
                //return 1;
        }
        for(;;) {
                char buffer[30];
                int len = read(fd, buffer, sizeof buffer -1);
                if (len > 0) {
                        buffer[len] = '\0';
                        int value = -1;
                        sscanf(buffer, "%d", &value);
                        printf("ADC Value: %d\n", value);
                        ui->lineEdit_Point->setText(QString::number(value));
                } else {
                        perror("read ADC device:");
                        //return 1;
                }
                usleep(500* 1000);
        }

        ::close(fd);
}
