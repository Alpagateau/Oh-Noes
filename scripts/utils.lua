
function TableString(o)
  local s = "{\n"

  for k,v in pairs(o) do
    s = s..k.." = "
    if type(v) == 'table' then
      s = s..TableString(v)
    end
    s = s..",\n"
  end
  s = s.."}"
  return s
end

function PrintTable(o)
  print("[LUA] "..TableString(o))
end
