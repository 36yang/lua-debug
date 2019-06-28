local thread = require "remotedebug.thread"

local threadMgr = [[
    package.path = %q
    package.cpath = %q
    local thread = require "remotedebug.thread"
    local MgrChanReq = thread.channel "NamedThread-Req:%s"
    local MgrClients = {}
    local MgrThreadHandle
    local function MgrResponse(tid, ...)
        local res = thread.channel("NamedThread-Res:"..tid)
        res:push(...)
    end
    local function MgrUpdate()
        while true do
            local ok, msg, id, data = MgrChanReq:pop()
            if not ok then
                return
            end
            if msg == "INIT" then
                MgrClients[id] = true
                MgrThreadHandle = data
            elseif msg == "EXIT" then
                MgrClients[id] = nil
                if next(MgrClients) == nil then
                    MgrResponse(id, "OK", MgrThreadHandle)
                    return true
                end
                MgrResponse(id, "BYE")
            end
            ::continue::
        end
    end
]]

local function reqChannelName(name)
    return "NamedThread-Req:"..name
end

local function resChannelName()
    return "NamedThread-Res:"..thread.id
end

local function createChannel(name)
    local ok, err = pcall(thread.newchannel, name)
    if not ok then
        if err:sub(1,17) ~= "Duplicate channel" then
            error(err)
        end
    end
    return not ok
end

local function createThread(name, path, cpath, script)
    if createChannel(reqChannelName(name)) then
        return
    end
    local thd = thread.thread(threadMgr:format(path, cpath, name) .. script)
    local reqChan = thread.channel(reqChannelName(name))
    reqChan:push("INIT", thread.id, thd)

    local errlog = thread.channel "errlog"
    local ok, msg = errlog:pop()
    if ok then
        print(msg)
    end
end

local function destoryThread(name)
    local reqChan = thread.channel(reqChannelName(name))
    local resChan = thread.channel(resChannelName())
    reqChan:push("EXIT", thread.id)
    local msg, handle = resChan:bpop()
    if msg == "OK" then
        thread.wait(handle)
    end
end


local function init()
    return not createChannel(resChannelName())
end

return {
    init = init,
    createChannel = createChannel,
    createThread = createThread,
    destoryThread = destoryThread,
}
