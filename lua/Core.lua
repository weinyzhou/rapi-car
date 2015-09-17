--[[
	lua 核心模块
]]--

require "class";


require "system.str";
local http=require "resty.http";
local cjson=require "cjson";
local core=class("Core");



core._VERSION = '0.1';

core.ip_info=nil;

core.ip_last=nil;
core.isp_last=nil;
core.session=false;
core.use_redis=false;
core.ipinfo_mysql=false;
core.redis=nil; 

function core.print_Err(self)
	ngx.header["Status"]="404 Not Found";
	ngx.status =ngx.HTTP_NOT_FOUND;
	ngx.exit(ngx.HTTP_NOT_FOUND);
	return false;
end

function core.getClientIP(self)
	local headers=ngx.req.get_headers();
	local clientip=headers["X-Real-IP"];
	if clientip then
		return clientip;
	end
	clientip=headers["x_forwarded_for"];
	if clientip then
		return clientip;
	end
	return ngx.var.remote_addr;
end
function core.getRequestTime(self)
	return ngx.req.start_time();
end

function core.getloadhtml(self,_URL,Referer)
	local httpc = http:new();
	 httpc:set_timeout(5000)
	local res, err=httpc:request_uri(_URL,{ 
		headers = {
		["User-Agent"]="Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36",
		["Referer"]=Referer,
				}
	});
	if not res then
        return nil;
     end
	if res.status==200 then
		return res.body;
	end
	return nil;
end

function core.getloadfile(self,_URL,filename)
	--读取body并写入文件
	local body_reader=function(reader,filename,size)
		local file,err=io.open(filename,"w+");
		if file==nil then
			ngx.log(ngx.ERR,"open file failed '"..filename.."' err="..err);
			return false;
		end
		local chunk, err;
		repeat
	        chunk, err = reader(8192)
	        if err then
	          ngx.log(ngx.ERR, err)
	          file:close();
	          return false;
	        end

	        if chunk then
	          file:write(chunk);
	        end
    	until not chunk
    	file:close();
    	return true;
	end
	local httpc = http:new();
	httpc:set_timeout(5000);
	local parsed_uri, err = httpc:parse_uri(_URL)
    if not parsed_uri then
        return false, err
    end
    local scheme, host, port, path = unpack(parsed_uri)
    
	local c,err=httpc:connect(host,port);
	if not c then
        return false, err
    end
    if scheme == "https" then
        local verify = true
        if params.ssl_verify == false then
            verify = false
        end
        local ok, err = httpc:ssl_handshake(nil, host, verify)
        if not ok then
            return false, err
        end
    end
    local res,err =httpc:request{
    	path=path,
    	headers = {
    	["Host"]=host,
    	["User-Agent"]="Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"
    	},
	};
	if not res then
        return false, err
    end

    local reader=res.body_reader;

    local ret=body_reader(reader,filename,8192);

   

    local ok, err = httpc:set_keepalive()
    if not ok then
        ngx_log(ngx_ERR, err)
    end

    return ret, nil

end


function core.explode(self,_str,seperator )
	local pos, arr = 0, {}  ;
        for st, sp in function() return string.find( _str, seperator, pos, true ) end do  
            table.insert( arr, string.sub( _str, pos, st-1 ) )  ;
            pos = sp + 1  ;
        end  
    table.insert( arr, string.sub( _str, pos ) )  ;
    return arr ;
end

function core.check_is_post(self)
	if ngx.var.request_method=="POST" then
		return true;
	end
	return false;
end

function core.server_move(self,url)
	ngx.header["Status"]="Found";
	ngx.header["Location"]=url;
	ngx.header.content_type="text/plain;charset=utf-8";
	ngx.status =ngx.HTTP_MOVED_TEMPORARILY ;
	ngx.exit(ngx.HTTP_MOVED_TEMPORARILY);
end

function core.loadfile(self,filename)
	local pFile=io.open(filename,"r");
	if pFile==nil then
		return nil;
	end
	local data=pFile:read("*a");
	pFile:close();
	return data;
end

function core.Exist(self,filename)
	local pFile=io.open(filename,"r");
	if pFile==nil then
		return false;
	end
	pFile:close();
	return true;
end

function core.server_successed(self)
	ngx.exit(200);
end

function core.convertUrlQuery(self,url)
	return ngx.decode_args(url,0);
end

function core.is_int(self,v)
	if type(v)=="number" then
		return true;
	end
	return false;
end

function core.is_bool(self,v)
	if type(v)=="boolean" then
		return true;
	end
	return false;
end

function core.getExptime(self)
	local cur=os.time();
	local curdd=os.date("*t");
	local dd={year=curdd.year, month=curdd.month, day=curdd.day, hour=23,min=59,sec=59,isdst=false};
	local out=os.time(dd);--失效时间+
	local exptime=out-cur+3600*14;

	return exptime;
end


function core.getContentType(self,ext,defmime)
	if defmime==nil then
		defmime="application/octet-stream";
	end
	ext=string.lower(ext);

	local mimes={
		jpg="image/jpeg",
		jpeg="image/jpeg",
		png="image/png",
		apk="application/vnd.android.package-archive",
		mp3="audio/mpeg",
		rmvb="video/vnd.rn-realvideo",
		txt="text/plain",
		wav="audio/x-wav",
		gif="image/gif",
		mp4="video/mp4",
		json="application/json; charset=utf-8",
		xml="application/xml; charset=utf-8",
		zip="application/zip",
		swf="application/x-shockwave-flash",
	};
	return mimes[ext];
end

return core;