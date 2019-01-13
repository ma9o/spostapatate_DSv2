char countEggs(unsigned b, unsigned c) {
    char ret;
    if (b) {
        if (c) {
            ret = "3";
        } else {
            ret = "2";
        }
    } else {
        if (c) {
            ret = "1";
        } else {
            ret = "0";
        }
    }
    return ret;
}

char countEggs(unsigned a, unsigned b, unsigned c) {
    char ret;
    if (a) {
        if (b) {
            if (c) {
                //nada
            } else {
                ret = "6";
            }
        } else {
            if (c) {
                ret = "5";
            } else {
                ret = "4";
            }
        }
    } else {
        ret = countEggs(b,c);
    }

    return ret;
}

