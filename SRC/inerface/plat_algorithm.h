#ifndef _PLAT_ALGORITHM_H
#define _PLAT_ALGORITHM_H

#define init_circular_list(plist_name, data_len, next) { \
int data_cur = 0; \
for(data_cur = 0; data_cur < data_len - 1; data_cur++) \
plist_name[data_cur].next = plist_name + data_cur + 1; \
plist_name[data_cur].next = plist_name; \
}



#endif
