local pm = ZFLuaImport("test_ZFLua_app_PageManager.lua");

local window = pm:embededCreateWindow();
window:windowShow();
pm:managerContainer():layoutParam():layoutMargin(ZFUIMarginMake(20));

ZFLuaImport("test_ZFLua_app_Page.lua", pm);

return window;

