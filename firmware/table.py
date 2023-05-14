#!/usr/bin/python

import random

table_x_len = 8
table_y_len = 8
table_sample = 512

f = open("table.c", "w")
f.write("#include <stdint.h>\n")
f.write("\n")
f.write("#define TABLE_X_LEN " + str(table_x_len) + "\n")
f.write("#define TABLE_Y_LEN " + str(table_y_len) + "\n")
f.write("#define TABLE_SAMPLE " + str(table_sample) + "\n")
f.write("\n")
f.write("const uint16_t table[TABLE_X_LEN][TABLE_Y_LEN][TABLE_SAMPLE] = {\n")

for x in range (table_x_len):
    f.write("\t{\n");

    for y in range (table_y_len):
        f.write("\t\t{")

        for spl in range (table_sample):

            if ((x == 0) and (y == 0)):
                f.write(str(int(spl*4096/table_sample)))
            elif ((x == 1) and (y == 0)):
                if (spl < (table_sample/2)):
                    f.write("0")
                else:
                    f.write("4095")
            else:
                f.write(str(random.randint(0, 4095)))

            if (spl != table_sample - 1):
                f.write(",")

        if (y == table_y_len-1):
            f.write("}\n")
        else:
            f.write("},\n")

    if (x == table_x_len-1):
        f.write("\t}\n");
    else:
        f.write("\t},\n");

f.write("};\n");
f.close()
