-- input from cpp
x = 0.0
y = 0.0
delta_time = 0.0

-- local variables
local speed = 3

counter = 0.0
co = coroutine.create(function ()
        while true do
            counter = delta_time + counter
            new_x = math.cos(counter * speed)
            new_y = math.sin(counter * speed)
            x = new_x
            y = new_y
            coroutine.yield()
        end
    end)

function start()
    print("started")
end

function update()
    print("x: ", tostring(x), ", y: ", tostring(y))
    coroutine.resume(co)
end