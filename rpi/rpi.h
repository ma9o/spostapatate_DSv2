#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 256


int GPIOExport(unsigned int gpio)
{
        int fd, len;
        char buf[MAX_BUF];

        fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
        if (fd < 0) {
                perror("gpio/export");
                return fd;
        }

        len = snprintf(buf, sizeof(buf), "%d", gpio);
        write(fd, buf, len);
        close(fd);

        return 0;
}


int GPIOUnexport(unsigned int gpio)
{
        int fd, len;
        char buf[MAX_BUF];

        fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
        if (fd < 0) {
                perror("gpio/export");
                return fd;
        }

        len = snprintf(buf, sizeof(buf), "%d", gpio);
        write(fd, buf, len);
        close(fd);
        return 0;
}


int GPIODirection(unsigned int gpio, unsigned char out_flag)
{
        int fd;
        char buf[MAX_BUF];

        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

        fd = open(buf, O_WRONLY);
        if (fd < 0) {
                perror("gpio/direction");
                return fd;
        }

        if (out_flag == 'o')
                write(fd, "out", 4);
        else if (out_flag == 'i')
                write(fd, "in", 3);

        close(fd);
        return 0;
}

int GPIOWrite(unsigned int gpio, unsigned char value)
{
        int fd;
        char buf[MAX_BUF];

        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

        fd = open(buf, O_WRONLY);
        if (fd < 0) {
                perror("gpio/set-value");
                return fd;
        }

        if (value)
                write(fd, "1", 2);
        else
                write(fd, "0", 2);

        close(fd);
        return 0;
}


int GPIORead(unsigned int gpio, unsigned char * value)
{
        int fd;
        char buf[MAX_BUF];
        char ch;

        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

        fd = open(buf, O_RDONLY);
        if (fd < 0) {
                perror("gpio/get-value");
                return fd;
        }

        read(fd, &ch, 1);

        if (ch != '0') {
                *value = '1';
        } else {
                *value = '0';
        }


        close(fd);
        return 0;
}
