--[[
	Name: Tiger nginx aes module
]]

local mcrypt = require("mcrypt");
local str = require "resty.string";


local defkey="0123456789abcdef";
local iv="chenxin(*&^%$##?";
function aes_encrypt(key,data)
	if key==nil then
		key=defkey;
	end
	mcrypt.module_open("rijndael-128", "", "cbc", iv);
	mcrypt.generic_init(key, iv);
	local ciphered=mcrypt.generic(data);
	mcrypt.generic_deinit();
	mcrypt.module_close();
	return str.to_hex(ciphered);
end
--[[
function string.fromhex(str)
     return (str:gsub('..', function (cc)
        return string.char(tonumber(cc, 16))
    end))
end
]]
function string.fromhex(str)
    local x = "";
    if string.sub(str,1,1)=="{" then
    	ngx.log(ngx.ERR,str);
    	return nil;
    end
    for y in str:gmatch('(..)') do
    	local num=tonumber(y, 16);
    	if num==nil then
    		return nil;
    	end
        x=x.. string.char(num);
    end
    return x;
end

function aes_decrypt(key,data)
	if key==nil then
		key=defkey;
	end
	data=string.fromhex(data);
	if data==nil then
		return nil;
	end
	mcrypt.module_open("rijndael-128", "", "cbc", iv);
	mcrypt.generic_init(key, iv);
	local ciphered=mcrypt.de_generic(data);
	mcrypt.generic_deinit();
	mcrypt.module_close();
	return ciphered;
end
