#ifndef _PLAT_ALGORITHM_H
#define _PLAT_ALGORITHM_H

#define init_circular_list(plist_name, data_len) { \
uint32_t data_cur = 0; \
for(data_cur = 0; data_cur < data_len - 1; data_cur++) \
plist_name[data_cur].next = plist_name + data_cur + 1; \
plist_name[data_cur].next = plist_name; \
}

#define init_circular_2dlist(plist_name, data_h_len, data_v_len) { \
uint32_t h_cur = 0, v_cur = 0; \
for(h_cur = 0; h_cur < data_h_len - 1; h_cur++) \
for(v_cur = 0; h_cur < data_v_len - 1; h_cur++) \
{ \
plist_name[h_cur][v_cur].v_next = plist_name[h_cur][v_cur + 1]; \
plist_name[h_cur][v_cur].p_next = plist_name[h_cur + 1][v_cur]; \
} \
for(h_cur = 0; h_cur < data_h_len; h_cur++) \
plist_name[h_cur][data_v_len - 1].v_next = plist_name[h_cur][0]; \
for(v_cur = 0; v_cur < data_v_len; v_cur++) \
plist_name[data_v_len - 1][v_cur].h_next = plist_name[0][v_cur]; \
}



#endif
