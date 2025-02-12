ilcnet_prot = Proto("ILCNET", "ILCNET PROTOCOL 1.0")

ilcnet_identifier = ProtoField.string("ILCNET.identifier", "Identifier")
ilcnet_intent = ProtoField.uint8("ILCNET.intent", "Intent", base.DEC)

ilcnet_session_id = ProtoField.string("ILCNET.sessionid", "Session ID")

-- Intent: CREATE
ilcnet_session_name_length = ProtoField.uint8("ILCNET.sessionnamelenght", "Session Name Length")
ilcnet_session_name = ProtoField.string("ILCNET.sessionname", "Session Name")




ilcnet_prot.fields = {ilcnet_identifier, ilcnet_intent, ilcnet_session_id, ilcnet_session_name_length, ilcnet_session_name }

function get_intent_name(intent) 
    local intent_name = "N/A"

    if intent == 0 then intent_name = "CREATE (Session)"
    elseif intent == 1 then intent_name = "JOIN (Session)"
    elseif intent == 2 then intent_name = "DESTROY (Session)"
    elseif intent == 3 then intent_name = "LEAVE (Session)"
    elseif intent == 4 then intent_name = "BEACON (Station)"
    elseif intent == 5 then intent_name = "BEACON (Session)"
    elseif intent == 6 then intent_name = "DISCOVER (Request)"
    elseif intent == 6 then intent_name = "DISCOVER (Response)"
    elseif intent == 7 then intent_name = "SESSION (Request)"
    elseif intent == 8 then intent_name = "SESSION (Response)"
    elseif intent == 12 then intent_name = "Session ID already in use (Session)"
    end

    return intent_name
end

function ilcnet_prot.dissector(buffer, pinfo, tree) 
    length = buffer:len();

    local subtree = tree:add(ilcnet_prot, buffer(), "ILCNET Prot 1.0")

    if length < 7 then
        subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Packetlength is under 7 bytes. This is the minimum requirement to send the ILCNET header and the Intent-ID! Lenght of the packet received: " .. length)
    end

    pinfo.cols.protocol = ilcnet_prot.name

    local test = buffer(0,6):string()
    
    if test ~= "ILCNET" then
        subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "The packet does not begin with \"ILCNET\"")
        return
    end

    
    
    local intentRaw = buffer(6,1);
    local intent = intentRaw:le_uint();
    local intent_name = get_intent_name(intent)

    subtree:add(ilcnet_identifier, buffer(0,6))
    subtree:add(ilcnet_intent, intentRaw, intent):append_text(" - " .. intent_name)


    if intent == 0 then -- INTENT: Create
        if length < 8 then
            subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Length of ILCNET Package not sufficient. Length should be at least 8, but is " .. length)
            return
        end
        

        local sessionIDRaw = buffer(7,1);
        local sessionID = sessionIDRaw:le_uint();

        local sessionNameLengthRaw = buffer(8,1);
        local sessionNameLength = sessionNameLengthRaw:le_uint()

        subtree:add(ilcnet_session_id, sessionIDRaw, sessionID);
        subtree:add(ilcnet_session_name_length, sessionNameLengthRaw, sessionNameLength);


        if length - 9 ~= sessionNameLength then
            subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Length of Session-Name field is not correct.. Length should be at " .. (length - 9) ..", but is " .. sessionNameLength)
            return
        end


        local sessionNameRaw = buffer(9,sessionNameLength);
        local sessionName = sessionNameRaw:string()

        subtree:add(ilcnet_session_name, sessionNameRaw, sessionName);
    elseif intent == 1 then 
    elseif intent == 2 then 
    elseif intent == 3 then 
    elseif intent == 4 then 
    elseif intent == 5 then -- INTENT: Beacon (Session)
        local sessionIDRaw = buffer(7,1)
        local sessionID = sessionIDRaw:le_uint()
        subtree:add(ilcnet_session_id, sessionIDRaw, sessionID);
    elseif intent == 6 then 
    elseif intent == 7 then 
    elseif intent == 8 then 
    elseif intent == 11 then -- INTENT: Station Query Version
        local stationVersion
    elseif intent == 12 then
        subtree:add("SessionID already used!", "", "ID in use!");
    else
        subtree:add_expert_info(PI_MALFORMED, PI_WARN, "INTENT is not known. IntentID that was transmitted: " .. intent) 
    end

end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(8900, ilcnet_prot)
