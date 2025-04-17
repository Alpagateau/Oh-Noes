
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

function MoveTowards(a, b, c)
  local d = (a-b)
  if math.abs(d) < c then
    return b
  else
    if d > 0 then
      return a - c
    else
      return a + c
    end
  end
end

function sign(a)
  if a > 0 then
    return 1
  else
    return -1
  end
end
