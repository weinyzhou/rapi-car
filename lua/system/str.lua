
function string.split(str,delim)  
    local i,j,k  
    local t = {}  
    k = 1  
    while true do  
        i,j = string.find(str,delim,k)  
        if i == nil then  
            table.insert(t,string.sub(str,k))  
            return t  
        end  
        table.insert(t,string.sub(str,k,i - 1))  
        k = j + 1  
    end  
end
--[[
function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
-- from PiL2 20.4

function trim2(s)
  return s:match "^%s*(.-)%s*$"
end
-- variant of trim1 (match)

function trim3(s)
  return s:gsub("^%s+", ""):gsub("%s+$", "")
end
-- two gsub's

function trim4(s)
  return s:match"^%s*(.*)":match"(.-)%s*$"
end
-- variant of trim3 (match)

function trim5(s)
  return s:match'^%s*(.*%S)' or ''
end
-- warning: has bad performance when s:match'^%s*$' and #s is large

function trim6(s)
  return s:match'^()%s*$' and '' or s:match'^%s*(.*%S)'
end
-- fixes performance problem in trim5.
-- note: the '()' avoids the overhead of default string capture.
-- This overhead is small, ~ 10% for successful whitespace match call
-- alone, and may not be noticeable in the overall benchmarks here,
-- but there's little harm either.  Instead replacing the first `match`
-- with a `find` has a similar effect, but that requires localizing
-- two functions in the trim7 variant below.

local match = string.match
function trim7(s)
  return match(s,'^()%s*$') and '' or match(s,'^%s*(.*%S)')
end
-- variant of trim6 (localize functions)

local find = string.find
local sub = string.sub
function trim8(s)
  local i1,i2 = find(s,'^%s*')
  if i2 >= i1 then s = sub(s,i2+1) end
  local i1,i2 = find(s,'%s*$')
  if i2 >= i1 then s = sub(s,1,i1-1) end
  return s
end
-- based on penlight 0.7.2

function trim9(s)
  local _,i1 = find(s,'^%s*')
  local i2 = find(s,'%s*$')
  return sub(s,i1+1,i2-1)
end
-- simplification of trim8

function trim10(s)
  local a = s:match('^%s*()')
  local b = s:match('()%s*$', a)
  return s:sub(a,b-1)
end
-- variant of trim9 (match)

function trim11(s)
 local n = s:find"%S"
 return n and s:match(".*%S", n) or ""
end
-- variant of trim6 (use n position)
-- http://lua-users.org/lists/lua-l/2009-12/msg00904.html
function trim12(s)
 local from = s:match"^%s*()"
 return from > #s and "" or s:match(".*%S", from)
end
]]
function string.trim(s)
 if not s then
    return nil;
 end
 local from = s:match"^%s*()"
 return from > #s and "" or s:match(".*%S", from)
end