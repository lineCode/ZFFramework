/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFUIColor_common.h
 * @brief common color
 */

#ifndef _ZFI_ZFUIColor_common_h_
#define _ZFI_ZFUIColor_common_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// common colors
/** @brief 0x000000FF */
#define ZFUIColorBlack() ZFUIColor(0x000000FF)
/** @brief 0x555555FF */
#define ZFUIColorGrayDark() ZFUIColor(0x555555FF)
/** @brief 0x808080FF */
#define ZFUIColorGray() ZFUIColor(0x808080FF)
/** @brief 0xAAAAAAFF */
#define ZFUIColorGrayLight() ZFUIColor(0xAAAAAAFF)
/** @brief 0xFFFFFFFF */
#define ZFUIColorWhite() ZFUIColor(0xFFFFFFFF)

/** @brief 0xFF0000FF */
#define ZFUIColorRed() ZFUIColor(0xFF0000FF)
/** @brief 0x00FF00FF */
#define ZFUIColorGreen() ZFUIColor(0x00FF00FF)
/** @brief 0x0000FFFF */
#define ZFUIColorBlue() ZFUIColor(0x0000FFFF)
/** @brief 0xFFFF00FF */
#define ZFUIColorYellow() ZFUIColor(0xFFFF00FF)
/** @brief 0xFF00FFFF */
#define ZFUIColorMagenta() ZFUIColor(0xFF00FFFF)
/** @brief 0x00FFFFFF */
#define ZFUIColorCyan() ZFUIColor(0x00FFFFFF)

// ============================================================
// web colors
/** @brief 0xF0F8FFFF */
#define ZFUIColor_webAliceBlue() ZFUIColorMake(0xF0, 0xF8, 0xFF)
/** @brief 0xFAEBD7FF */
#define ZFUIColor_webAntiqueWhite() ZFUIColorMake(0xFA, 0xEB, 0xD7)
/** @brief 0x00FFFFFF */
#define ZFUIColor_webAqua() ZFUIColorMake(0x00, 0xFF, 0xFF)
/** @brief 0x7FFFD4FF */
#define ZFUIColor_webAquamarine() ZFUIColorMake(0x7F, 0xFF, 0xD4)
/** @brief 0xF0FFFFFF */
#define ZFUIColor_webAzure() ZFUIColorMake(0xF0, 0xFF, 0xFF)
/** @brief 0xF5F5DCFF */
#define ZFUIColor_webBeige() ZFUIColorMake(0xF5, 0xF5, 0xDC)
/** @brief 0xFFE4C4FF */
#define ZFUIColor_webBisque() ZFUIColorMake(0xFF, 0xE4, 0xC4)
/** @brief 0x000000FF */
#define ZFUIColor_webBlack() ZFUIColorMake(0x00, 0x00, 0x00)
/** @brief 0xFFEBCDFF */
#define ZFUIColor_webBlanchedAlmond() ZFUIColorMake(0xFF, 0xEB, 0xCD)
/** @brief 0x0000FFFF */
#define ZFUIColor_webBlue() ZFUIColorMake(0x00, 0x00, 0xFF)
/** @brief 0x8A2BE2FF */
#define ZFUIColor_webBlueViolet() ZFUIColorMake(0x8A, 0x2B, 0xE2)
/** @brief 0xA52A2AFF */
#define ZFUIColor_webBrown() ZFUIColorMake(0xA5, 0x2A, 0x2A)
/** @brief 0xDEB887FF */
#define ZFUIColor_webBurlyWood() ZFUIColorMake(0xDE, 0xB8, 0x87)
/** @brief 0x5F9EA0FF */
#define ZFUIColor_webCadetBlue() ZFUIColorMake(0x5F, 0x9E, 0xA0)
/** @brief 0x7FFF00FF */
#define ZFUIColor_webChartreuse() ZFUIColorMake(0x7F, 0xFF, 0x00)
/** @brief 0xD2691EFF */
#define ZFUIColor_webChocolate() ZFUIColorMake(0xD2, 0x69, 0x1E)
/** @brief 0xFF7F50FF */
#define ZFUIColor_webCoral() ZFUIColorMake(0xFF, 0x7F, 0x50)
/** @brief 0x6495EDFF */
#define ZFUIColor_webCornflowerBlue() ZFUIColorMake(0x64, 0x95, 0xED)
/** @brief 0xFFF8DCFF */
#define ZFUIColor_webCornsilk() ZFUIColorMake(0xFF, 0xF8, 0xDC)
/** @brief 0xDC143CFF */
#define ZFUIColor_webCrimson() ZFUIColorMake(0xDC, 0x14, 0x3C)
/** @brief 0x00FFFFFF */
#define ZFUIColor_webCyan() ZFUIColorMake(0x00, 0xFF, 0xFF)
/** @brief 0x00008BFF */
#define ZFUIColor_webDarkBlue() ZFUIColorMake(0x00, 0x00, 0x8B)
/** @brief 0x008B8BFF */
#define ZFUIColor_webDarkCyan() ZFUIColorMake(0x00, 0x8B, 0x8B)
/** @brief 0xB8860BFF */
#define ZFUIColor_webDarkGoldenRod() ZFUIColorMake(0xB8, 0x86, 0x0B)
/** @brief 0xA9A9A9FF */
#define ZFUIColor_webDarkGray() ZFUIColorMake(0xA9, 0xA9, 0xA9)
/** @brief 0x006400FF */
#define ZFUIColor_webDarkGreen() ZFUIColorMake(0x00, 0x64, 0x00)
/** @brief 0xBDB76BFF */
#define ZFUIColor_webDarkKhaki() ZFUIColorMake(0xBD, 0xB7, 0x6B)
/** @brief 0x8B008BFF */
#define ZFUIColor_webDarkMagenta() ZFUIColorMake(0x8B, 0x00, 0x8B)
/** @brief 0x556B2FFF */
#define ZFUIColor_webDarkOliveGreen() ZFUIColorMake(0x55, 0x6B, 0x2F)
/** @brief 0xFF8C00FF */
#define ZFUIColor_webDarkOrange() ZFUIColorMake(0xFF, 0x8C, 0x00)
/** @brief 0x9932CCFF */
#define ZFUIColor_webDarkOrchid() ZFUIColorMake(0x99, 0x32, 0xCC)
/** @brief 0x8B0000FF */
#define ZFUIColor_webDarkRed() ZFUIColorMake(0x8B, 0x00, 0x00)
/** @brief 0xE9967AFF */
#define ZFUIColor_webDarkSalmon() ZFUIColorMake(0xE9, 0x96, 0x7A)
/** @brief 0x8FBC8FFF */
#define ZFUIColor_webDarkSeaGreen() ZFUIColorMake(0x8F, 0xBC, 0x8F)
/** @brief 0x483D8BFF */
#define ZFUIColor_webDarkSlateBlue() ZFUIColorMake(0x48, 0x3D, 0x8B)
/** @brief 0x2F4F4FFF */
#define ZFUIColor_webDarkSlateGray() ZFUIColorMake(0x2F, 0x4F, 0x4F)
/** @brief 0x00CED1FF */
#define ZFUIColor_webDarkTurquoise() ZFUIColorMake(0x00, 0xCE, 0xD1)
/** @brief 0x9400D3FF */
#define ZFUIColor_webDarkViolet() ZFUIColorMake(0x94, 0x00, 0xD3)
/** @brief 0xFF1493FF */
#define ZFUIColor_webDeepPink() ZFUIColorMake(0xFF, 0x14, 0x93)
/** @brief 0x00BFFFFF */
#define ZFUIColor_webDeepSkyBlue() ZFUIColorMake(0x00, 0xBF, 0xFF)
/** @brief 0x696969FF */
#define ZFUIColor_webDimGray() ZFUIColorMake(0x69, 0x69, 0x69)
/** @brief 0x1E90FFFF */
#define ZFUIColor_webDodgerBlue() ZFUIColorMake(0x1E, 0x90, 0xFF)
/** @brief 0xB22222FF */
#define ZFUIColor_webFireBrick() ZFUIColorMake(0xB2, 0x22, 0x22)
/** @brief 0xFFFAF0FF */
#define ZFUIColor_webFloralWhite() ZFUIColorMake(0xFF, 0xFA, 0xF0)
/** @brief 0x228B22FF */
#define ZFUIColor_webForestGreen() ZFUIColorMake(0x22, 0x8B, 0x22)
/** @brief 0xFF00FFFF */
#define ZFUIColor_webFuchsia() ZFUIColorMake(0xFF, 0x00, 0xFF)
/** @brief 0xDCDCDCFF */
#define ZFUIColor_webGainsboro() ZFUIColorMake(0xDC, 0xDC, 0xDC)
/** @brief 0xF8F8FFFF */
#define ZFUIColor_webGhostWhite() ZFUIColorMake(0xF8, 0xF8, 0xFF)
/** @brief 0xFFD700FF */
#define ZFUIColor_webGold() ZFUIColorMake(0xFF, 0xD7, 0x00)
/** @brief 0xDAA520FF */
#define ZFUIColor_webGoldenRod() ZFUIColorMake(0xDA, 0xA5, 0x20)
/** @brief 0x808080FF */
#define ZFUIColor_webGray() ZFUIColorMake(0x80, 0x80, 0x80)
/** @brief 0x008000FF */
#define ZFUIColor_webGreen() ZFUIColorMake(0x00, 0x80, 0x00)
/** @brief 0xADFF2FFF */
#define ZFUIColor_webGreenYellow() ZFUIColorMake(0xAD, 0xFF, 0x2F)
/** @brief 0xF0FFF0FF */
#define ZFUIColor_webHoneyDew() ZFUIColorMake(0xF0, 0xFF, 0xF0)
/** @brief 0xFF69B4FF */
#define ZFUIColor_webHotPink() ZFUIColorMake(0xFF, 0x69, 0xB4)
/** @brief 0xCD5C5CFF */
#define ZFUIColor_webIndianRed() ZFUIColorMake(0xCD, 0x5C, 0x5C)
/** @brief 0x4B0082FF */
#define ZFUIColor_webIndigo() ZFUIColorMake(0x4B, 0x00, 0x82)
/** @brief 0xFFFFF0FF */
#define ZFUIColor_webIvory() ZFUIColorMake(0xFF, 0xFF, 0xF0)
/** @brief 0xF0E68CFF */
#define ZFUIColor_webKhaki() ZFUIColorMake(0xF0, 0xE6, 0x8C)
/** @brief 0xE6E6FAFF */
#define ZFUIColor_webLavender() ZFUIColorMake(0xE6, 0xE6, 0xFA)
/** @brief 0xFFF0F5FF */
#define ZFUIColor_webLavenderBlush() ZFUIColorMake(0xFF, 0xF0, 0xF5)
/** @brief 0x7CFC00FF */
#define ZFUIColor_webLawnGreen() ZFUIColorMake(0x7C, 0xFC, 0x00)
/** @brief 0xFFFACDFF */
#define ZFUIColor_webLemonChiffon() ZFUIColorMake(0xFF, 0xFA, 0xCD)
/** @brief 0xADD8E6FF */
#define ZFUIColor_webLightBlue() ZFUIColorMake(0xAD, 0xD8, 0xE6)
/** @brief 0xF08080FF */
#define ZFUIColor_webLightCoral() ZFUIColorMake(0xF0, 0x80, 0x80)
/** @brief 0xE0FFFFFF */
#define ZFUIColor_webLightCyan() ZFUIColorMake(0xE0, 0xFF, 0xFF)
/** @brief 0xFAFAD2FF */
#define ZFUIColor_webLightGoldenRodYellow() ZFUIColorMake(0xFA, 0xFA, 0xD2)
/** @brief 0xD3D3D3FF */
#define ZFUIColor_webLightGray() ZFUIColorMake(0xD3, 0xD3, 0xD3)
/** @brief 0x90EE90FF */
#define ZFUIColor_webLightGreen() ZFUIColorMake(0x90, 0xEE, 0x90)
/** @brief 0xFFB6C1FF */
#define ZFUIColor_webLightPink() ZFUIColorMake(0xFF, 0xB6, 0xC1)
/** @brief 0xFFA07AFF */
#define ZFUIColor_webLightSalmon() ZFUIColorMake(0xFF, 0xA0, 0x7A)
/** @brief 0x20B2AAFF */
#define ZFUIColor_webLightSeaGreen() ZFUIColorMake(0x20, 0xB2, 0xAA)
/** @brief 0x87CEFAFF */
#define ZFUIColor_webLightSkyBlue() ZFUIColorMake(0x87, 0xCE, 0xFA)
/** @brief 0x778899FF */
#define ZFUIColor_webLightSlateGray() ZFUIColorMake(0x77, 0x88, 0x99)
/** @brief 0xB0C4DEFF */
#define ZFUIColor_webLightSteelBlue() ZFUIColorMake(0xB0, 0xC4, 0xDE)
/** @brief 0xFFFFE0FF */
#define ZFUIColor_webLightYellow() ZFUIColorMake(0xFF, 0xFF, 0xE0)
/** @brief 0x00FF00FF */
#define ZFUIColor_webLime() ZFUIColorMake(0x00, 0xFF, 0x00)
/** @brief 0x32CD32FF */
#define ZFUIColor_webLimeGreen() ZFUIColorMake(0x32, 0xCD, 0x32)
/** @brief 0xFAF0E6FF */
#define ZFUIColor_webLinen() ZFUIColorMake(0xFA, 0xF0, 0xE6)
/** @brief 0xFF00FFFF */
#define ZFUIColor_webMagenta() ZFUIColorMake(0xFF, 0x00, 0xFF)
/** @brief 0x800000FF */
#define ZFUIColor_webMaroon() ZFUIColorMake(0x80, 0x00, 0x00)
/** @brief 0x66CDAAFF */
#define ZFUIColor_webMediumAquaMarine() ZFUIColorMake(0x66, 0xCD, 0xAA)
/** @brief 0x0000CDFF */
#define ZFUIColor_webMediumBlue() ZFUIColorMake(0x00, 0x00, 0xCD)
/** @brief 0xBA55D3FF */
#define ZFUIColor_webMediumOrchid() ZFUIColorMake(0xBA, 0x55, 0xD3)
/** @brief 0x9370DBFF */
#define ZFUIColor_webMediumPurple() ZFUIColorMake(0x93, 0x70, 0xDB)
/** @brief 0x3CB371FF */
#define ZFUIColor_webMediumSeaGreen() ZFUIColorMake(0x3C, 0xB3, 0x71)
/** @brief 0x7B68EEFF */
#define ZFUIColor_webMediumSlateBlue() ZFUIColorMake(0x7B, 0x68, 0xEE)
/** @brief 0x00FA9AFF */
#define ZFUIColor_webMediumSpringGreen() ZFUIColorMake(0x00, 0xFA, 0x9A)
/** @brief 0x48D1CCFF */
#define ZFUIColor_webMediumTurquoise() ZFUIColorMake(0x48, 0xD1, 0xCC)
/** @brief 0xC71585FF */
#define ZFUIColor_webMediumVioletRed() ZFUIColorMake(0xC7, 0x15, 0x85)
/** @brief 0x191970FF */
#define ZFUIColor_webMidnightBlue() ZFUIColorMake(0x19, 0x19, 0x70)
/** @brief 0xF5FFFAFF */
#define ZFUIColor_webMintCream() ZFUIColorMake(0xF5, 0xFF, 0xFA)
/** @brief 0xFFE4E1FF */
#define ZFUIColor_webMistyRose() ZFUIColorMake(0xFF, 0xE4, 0xE1)
/** @brief 0xFFE4B5FF */
#define ZFUIColor_webMoccasin() ZFUIColorMake(0xFF, 0xE4, 0xB5)
/** @brief 0xFFDEADFF */
#define ZFUIColor_webNavajoWhite() ZFUIColorMake(0xFF, 0xDE, 0xAD)
/** @brief 0x000080FF */
#define ZFUIColor_webNavy() ZFUIColorMake(0x00, 0x00, 0x80)
/** @brief 0xFDF5E6FF */
#define ZFUIColor_webOldLace() ZFUIColorMake(0xFD, 0xF5, 0xE6)
/** @brief 0x808000FF */
#define ZFUIColor_webOlive() ZFUIColorMake(0x80, 0x80, 0x00)
/** @brief 0x6B8E23FF */
#define ZFUIColor_webOliveDrab() ZFUIColorMake(0x6B, 0x8E, 0x23)
/** @brief 0xFFA500FF */
#define ZFUIColor_webOrange() ZFUIColorMake(0xFF, 0xA5, 0x00)
/** @brief 0xFF4500FF */
#define ZFUIColor_webOrangeRed() ZFUIColorMake(0xFF, 0x45, 0x00)
/** @brief 0xDA70D6FF */
#define ZFUIColor_webOrchid() ZFUIColorMake(0xDA, 0x70, 0xD6)
/** @brief 0xEEE8AAFF */
#define ZFUIColor_webPaleGoldenRod() ZFUIColorMake(0xEE, 0xE8, 0xAA)
/** @brief 0x98FB98FF */
#define ZFUIColor_webPaleGreen() ZFUIColorMake(0x98, 0xFB, 0x98)
/** @brief 0xAFEEEEFF */
#define ZFUIColor_webPaleTurquoise() ZFUIColorMake(0xAF, 0xEE, 0xEE)
/** @brief 0xDB7093FF */
#define ZFUIColor_webPaleVioletRed() ZFUIColorMake(0xDB, 0x70, 0x93)
/** @brief 0xFFEFD5FF */
#define ZFUIColor_webPapayaWhip() ZFUIColorMake(0xFF, 0xEF, 0xD5)
/** @brief 0xFFDAB9FF */
#define ZFUIColor_webPeachPuff() ZFUIColorMake(0xFF, 0xDA, 0xB9)
/** @brief 0xCD853FFF */
#define ZFUIColor_webPeru() ZFUIColorMake(0xCD, 0x85, 0x3F)
/** @brief 0xFFC0CBFF */
#define ZFUIColor_webPink() ZFUIColorMake(0xFF, 0xC0, 0xCB)
/** @brief 0xDDA0DDFF */
#define ZFUIColor_webPlum() ZFUIColorMake(0xDD, 0xA0, 0xDD)
/** @brief 0xB0E0E6FF */
#define ZFUIColor_webPowderBlue() ZFUIColorMake(0xB0, 0xE0, 0xE6)
/** @brief 0x800080FF */
#define ZFUIColor_webPurple() ZFUIColorMake(0x80, 0x00, 0x80)
/** @brief 0xFF0000FF */
#define ZFUIColor_webRed() ZFUIColorMake(0xFF, 0x00, 0x00)
/** @brief 0xBC8F8FFF */
#define ZFUIColor_webRosyBrown() ZFUIColorMake(0xBC, 0x8F, 0x8F)
/** @brief 0x4169E1FF */
#define ZFUIColor_webRoyalBlue() ZFUIColorMake(0x41, 0x69, 0xE1)
/** @brief 0x8B4513FF */
#define ZFUIColor_webSaddleBrown() ZFUIColorMake(0x8B, 0x45, 0x13)
/** @brief 0xFA8072FF */
#define ZFUIColor_webSalmon() ZFUIColorMake(0xFA, 0x80, 0x72)
/** @brief 0xF4A460FF */
#define ZFUIColor_webSandyBrown() ZFUIColorMake(0xF4, 0xA4, 0x60)
/** @brief 0x2E8B57FF */
#define ZFUIColor_webSeaGreen() ZFUIColorMake(0x2E, 0x8B, 0x57)
/** @brief 0xFFF5EEFF */
#define ZFUIColor_webSeaShell() ZFUIColorMake(0xFF, 0xF5, 0xEE)
/** @brief 0xA0522DFF */
#define ZFUIColor_webSienna() ZFUIColorMake(0xA0, 0x52, 0x2D)
/** @brief 0xC0C0C0FF */
#define ZFUIColor_webSilver() ZFUIColorMake(0xC0, 0xC0, 0xC0)
/** @brief 0x87CEEBFF */
#define ZFUIColor_webSkyBlue() ZFUIColorMake(0x87, 0xCE, 0xEB)
/** @brief 0x6A5ACDFF */
#define ZFUIColor_webSlateBlue() ZFUIColorMake(0x6A, 0x5A, 0xCD)
/** @brief 0x708090FF */
#define ZFUIColor_webSlateGray() ZFUIColorMake(0x70, 0x80, 0x90)
/** @brief 0xFFFAFAFF */
#define ZFUIColor_webSnow() ZFUIColorMake(0xFF, 0xFA, 0xFA)
/** @brief 0x00FF7FFF */
#define ZFUIColor_webSpringGreen() ZFUIColorMake(0x00, 0xFF, 0x7F)
/** @brief 0x4682B4FF */
#define ZFUIColor_webSteelBlue() ZFUIColorMake(0x46, 0x82, 0xB4)
/** @brief 0xD2B48CFF */
#define ZFUIColor_webTan() ZFUIColorMake(0xD2, 0xB4, 0x8C)
/** @brief 0x008080FF */
#define ZFUIColor_webTeal() ZFUIColorMake(0x00, 0x80, 0x80)
/** @brief 0xD8BFD8FF */
#define ZFUIColor_webThistle() ZFUIColorMake(0xD8, 0xBF, 0xD8)
/** @brief 0xFF6347FF */
#define ZFUIColor_webTomato() ZFUIColorMake(0xFF, 0x63, 0x47)
/** @brief 0x40E0D0FF */
#define ZFUIColor_webTurquoise() ZFUIColorMake(0x40, 0xE0, 0xD0)
/** @brief 0xEE82EEFF */
#define ZFUIColor_webViolet() ZFUIColorMake(0xEE, 0x82, 0xEE)
/** @brief 0xF5DEB3FF */
#define ZFUIColor_webWheat() ZFUIColorMake(0xF5, 0xDE, 0xB3)
/** @brief 0xFFFFFFFF */
#define ZFUIColor_webWhite() ZFUIColorMake(0xFF, 0xFF, 0xFF)
/** @brief 0xF5F5F5FF */
#define ZFUIColor_webWhiteSmoke() ZFUIColorMake(0xF5, 0xF5, 0xF5)
/** @brief 0xFFFF00FF */
#define ZFUIColor_webYellow() ZFUIColorMake(0xFF, 0xFF, 0x00)
/** @brief 0x9ACD32FF */
#define ZFUIColor_webYellowGreen() ZFUIColorMake(0x9A, 0xCD, 0x32)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIColor_common_h_

