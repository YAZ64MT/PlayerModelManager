#ifndef PROXYMM_KV_H
#define PROXYMM_KV_H

#include "modding.h"
#include "global.h"

// Global
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Get(const char* key, void* dest, u32 size));
RECOMP_IMPORT("ProxyMM_KV", u32  KV_Global_Get_U32(const char* key, u32 defaultValue));
RECOMP_IMPORT("ProxyMM_KV", s32  KV_Global_Get_S32(const char* key, s32 defaultValue));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set(const char* key, void* data, u32 size));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set_U8(const char* key, u8 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set_S8(const char* key, s8 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set_U16(const char* key, u16 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set_S16(const char* key, s16 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set_U32(const char* key, u32 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Set_S32(const char* key, s32 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Has(const char* key));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Global_Remove(const char* key));

// Save Slot based
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Get(const char* key, void* dest, u32 size));
RECOMP_IMPORT("ProxyMM_KV", u32  KV_Slot_Get_U32(const char* key, u32 defaultValue));
RECOMP_IMPORT("ProxyMM_KV", s32  KV_Slot_Get_S32(const char* key, s32 defaultValue));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set(const char* key, void* data, u32 size));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set_U8(const char* key, u8 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set_S8(const char* key, s8 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set_U16(const char* key, u16 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set_S16(const char* key, s16 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set_U32(const char* key, u32 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Set_S32(const char* key, s32 value));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Has(const char* key));
RECOMP_IMPORT("ProxyMM_KV", bool KV_Slot_Remove(const char* key));

#endif // PROXYMM_KV_H
