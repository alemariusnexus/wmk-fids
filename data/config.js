{
    "simulationTimeStart" : "10:17",
    "simulationTimePart2" : "14:36",
    "simulationTimeScale" : 1.0,
    /*"simulationTimePresets" : [
        { "name" : "Part 1", "time" : "14:00" },
        { "name" : "Part 2", "time" : "16:35" }
    ],*/
    "curTimeFormat" : "HH:mm",
    "fidsTimeFormat" : "HH:mm",
    "expectedDepartureDisplayThreshold" : 300,
    "maxDisplayedFlights" : -1,
    "blinkInterval" : 1000,
    "blinkTimeCancelled" : 15000,
    "blinkTimeBoarding" : 15000,
    "blinkTimeGateClosing" : 15000,
    "weatherDefault" : "rainy",
    "weatherPart2" : "clear",
    "weather" : [
        { "id" : "clear", "icon" : "icons/weather-clear.png", "iconScale" : 80 },
        { "id" : "rainy", "icon" : "icons/weather-rainy.png", "iconScale" : 80 }
    ],
    
    "simulation" : {
        "beginBoardingTimeDeviation" : 30,
        "boardingTimeAverage" : 600,
        "boardingTimeDeviation" : 180,
        "boardingTimeDeviation" : 0,
        "gateCloseTime" : 120,
        "expectedDepartureTimeModulo" : 300,
        "expectedDepartureTimeHysteresis" : 30,
        "departLingerTime" : 180,
        "delayMax" : 7200,
        "cancelInterval" : 3000,
        "cancelTargetNum" : 15,
        "cancelNumMin" : 10,
        "cancelNumMax" : 10,
        //"loadCutoffTime" : 300,
        //"loadCutoffTime" : 60,
        "loadCutoffTime" : 600,
        
        "events" : [
            {
                "intervalMin" : 600,
                "intervalMax" : 1800,
                "freshTriggersMin" : 2,
                "freshTriggersMax" : 4,
                "type" : "delay",
                //"status" : [ "Scheduled", "Boarding", "GateClosing", "Departed" ],
                "status" : [ "Scheduled" ],
                "farAffectedTime" : 3600,
                "delayMin" : 300,
                "delayMax" : 1800
            }
        ]
    },
    
    
    "remarkBoardingColor" : "#00ff00",
    "remarkGateClosingColor" : "#ff6000",
    "remarkDepartedColor" : "#ffffff",
    "remarkCancelledColor" : "#ff0000",
    "globalCSS" : "background-color: #010d35; color: #ffffff; font-family: \"Monospace\";",
    "titleText" : "ABFLUG / DEPARTURES",
    "titleCSS" : "",
    "titleLabelCSS" : "margin-left: 25px; font-size: 44pt;",
    "titleTimeCSS" : "font-size: 60pt; font-weight: bold;",
    "titleIconScale" : 128,
    "weatherIconCSS" : "margin-right: 100px;",
    "headerTitles" : [
        "Planmäßig\nScheduled",
        "Voraussichtlich\nEstimated",
        "Nach\nTo",
        "Flug\nFlight",
        "Gate",
        "Anmerkungen\nRemarks"
    ],
    "headerHeight" : 80,
    "tableHeaderCSS" : "QHeaderView::section { border: 0px solid black; background-color: #00fffc; font-size: 18pt; padding-left: 5px; }",
    "tableCSS" : "",
    "tablePaddingLeft" : 10,
    "tablePaddingRight" : 40,
    "rowSchemes" : [
        { "backgroundColor" : "#010d35", "textColor" : "#ffffff", "font" : { "size" : 32 } },
        { "backgroundColor" : "#0072ef", "textColor" : "#ffffff", "font" : { "size" : 32 } }
    ],
    "columnSizes" : [
        //  SHEDTIME    EXPTIME     DEST        FLIGHTNO    GATE        REMARKS
            1,          1,          2,          1,          1,          2
    ],
    
    
    
    "flights" : [
        [ "04:45", "Moscow", "RU310", "C18" ],
        [ "05:15", "Jerez", "X32272", "A58" ],
        [ "06:00", "Palma de Mallorca", "X32684", "A2" ],
        [ "06:05", "Lisbon", "TP573", "D35" ],
        [ "06:10", "Palma de Mallorca", "X32432", "B14" ],
        [ "06:15", "Munich", "LH92", "A40" ],
        [ "06:15", "Berlin", "LH172", "Z33" ],
        [ "06:30", "Porto", "FR1536", "A25" ],
        [ "06:30", "Madrid", "IB8713", "Z48" ],
        [ "06:30", "Hamburg", "LH2", "B42" ],
        [ "06:35", "Zurich", "LH1182", "A53" ],
        [ "06:40", "Bremen", "LH350", "B5" ],
        [ "06:40", "Stockholm", "LH800", "B32" ],
        [ "06:40", "Dublin", "FR381", "D48" ],
        [ "06:45", "Berlin", "LH170", "B61" ],
        [ "06:45", "Dublin", "LH976", "B20" ],
        [ "06:50", "Paris", "LH1026", "E6" ],
        [ "06:52", "-", "LH3400", "Z39" ],
        [ "06:55", "Amsterdam", "KL1762", "C14" ],
        [ "06:55", "Hannover", "LH46", "A16" ],
        [ "07:00", "London", "BA8731", "A66" ],
        [ "07:00", "Palma de Mallorca", "FR4140", "Z16" ],
        [ "07:00", "Hamburg", "LH4", "Z14" ],
        [ "07:00", "London", "LH924", "A9" ],
        [ "07:00", "Amsterdam", "LH986", "B58" ],
        [ "07:00", "Warsaw", "LH1346", "A18" ],
        [ "07:00", "Warsaw", "LO384", "Z15" ],
        [ "07:00", "London", "FR1686", "D37" ],
        [ "07:05", "Leipzig", "LH154", "B62" ],
        [ "07:05", "Dresden", "LH204", "D32" ],
        [ "07:05", "Paris", "AF1019", "A29" ],
        [ "07:05", "Nurnberg", "LH140", "A14" ],
        [ "07:05", "Oslo", "LH866", "A18" ],
        [ "07:05", "London", "LH926", "A10" ],
        [ "07:09", "-", "LH3600", "Z48" ],
        [ "07:10", "Abu Dhabi", "EY902", "A66" ],
        [ "07:10", "Stuttgart", "LH126", "A67" ],
        [ "07:10", "Geneva", "LH1212", "B25" ],
        [ "07:10", "Dusseldorf", "LH72", "Z65" ],
        [ "07:10", "Barcelona", "LH1124", "B45" ],
        [ "07:15", "Munich", "LH94", "B6" ],
        [ "07:20", "Rome", "LH230", "E24" ],
        [ "07:25", "Luxembourg", "LH390", "B18" ],
        [ "07:25", "Brussels", "LH1004", "A27" ],
        [ "07:25", "-", "LH3500", "A38" ],
        [ "07:30", "London", "BA901", "B43" ],
        [ "07:30", "Milan", "LH248", "D41" ],
        [ "07:30", "Birmingham", "LH952", "B22" ],
        [ "07:40", "Hurghada", "XG2341", "A26" ],
        [ "07:40", "Istanbul", "TK1598", "B11" ],
        [ "07:45", "Berlin", "LH174", "Z11" ],
        [ "07:50", "Vienna", "OS128", "A58" ],
        [ "07:50", "London", "BA3385", "Z56" ],
        [ "07:52", "Mannheim", "LH3430", "Z47" ],
        [ "07:55", "Tenerife", "DE3616", "A51" ],
        [ "07:55", "Copenhagen", "LH824", "A15" ],
        [ "07:55", "London", "LH900", "Z12" ],
        [ "07:55", "Manchester", "LH940", "C5" ],
        [ "08:00", "Hamburg", "LH6", "D7" ],
        [ "08:00", "Zurich", "LH1184", "B7" ],
        [ "08:05", "Gothenburg", "LH812", "A56" ],
        [ "08:05", "Florence", "LH308", "E18" ],
        [ "08:10", "Fuerteventura", "DE1402", "Z46" ],
        [ "08:10", "Istanbul", "TK6404", "Z41" ],
        [ "08:10", "Graz", "OS252", "D44" ],
        [ "08:15", "Dubai", "DE2358", "B52" ],
        [ "08:15", "Berlin", "LH38", "A5" ],
        [ "08:15", "Munich", "LH96", "B50" ],
        [ "08:20", "Turin", "EN8842", "Z46" ],
        [ "08:20", "Nice", "LH1058", "B9" ],
        [ "08:20", "Marseille", "LH1086", "A35" ],
        [ "08:20", "Paris", "LH1028", "C14" ],
        [ "08:20", "Krakow", "LH1364", "Z15" ],
        [ "08:20", "New York", "SQ26", "B53" ],
        [ "08:30", "Bologna", "LH282", "A40" ],
        [ "08:30", "Budapest", "LH1334", "Z24" ],
        [ "08:30", "Berlin", "U25542", "A23" ],
        [ "08:35", "Chicago", "UA945", "D11" ],
        [ "08:40", "Amsterdam", "CX38", "B55" ],
        [ "08:40", "Bridgetown", "DE3782", "A22" ],
        [ "08:45", "Milan", "LH270", "A65" ],
        [ "08:45", "Venice", "LH324", "D54" ],
        [ "08:45", "Berlin", "LH176", "A11" ],
        [ "08:45", "Poznan", "LH1388", "D55" ],
        [ "08:45", "Zurich", "LX1069", "D40" ],
        [ "08:50", "Billund", "LH836", "E7" ],
        [ "08:50", "Palma de Mallorca", "LH1154", "E2" ],
        [ "08:50", "Vienna", "LH1234", "Z26" ],
        [ "08:50", "Innsbruck", "OS278", "D41" ],
        [ "08:50", "Prague", "LH1392", "A52" ],
        [ "08:55", "Lyon", "LH1074", "Z57" ],
        [ "09:00", "Linz", "EN8040", "D39" ],
        [ "09:00", "Friedrichshafen", "LH370", "A56" ],
        [ "09:00", "London", "LH902", "C5" ],
        [ "09:00", "Amsterdam", "LH988", "Z31" ],
        [ "09:00", "Hamburg", "LH8", "D38" ],
        [ "09:00", "London", "LH928", "B37" ],
        [ "09:00", "Salzburg", "OS262", "B16" ],
        [ "09:05", "Hannover", "LH48", "D18" ],
        [ "09:05", "Wroclaw", "LH1358", "D45" ],
        [ "09:05", "Geneva", "LX1081", "D6" ],
        [ "09:10", "Munster", "LH62", "Z11" ],
        [ "09:10", "Milan", "LH246", "Z45" ],
        [ "09:10", "Genoa", "LH260", "A60" ],
        [ "09:10", "Bremen", "LH352", "A17" ],
        [ "09:10", "Lisbon", "LH1166", "D47" ],
        [ "09:10", "Milan", "AZ413", "Z67" ],
        [ "09:10", "Basel", "LH1202", "E22" ],
        [ "09:10", "Prague", "OK535", "Z17" ],
        [ "09:15", "Leipzig", "LH156", "Z34" ],
        [ "09:15", "Dresden", "LH206", "A9" ],
        [ "09:15", "Paris", "LH1030", "E12" ],
        [ "09:15", "Shanghai", "LH8402", "C4" ],
        [ "09:15", "Munich", "LH98", "B52" ],
        [ "09:15", "Ljubljana", "LH1460", "A68" ],
        [ "09:20", "Dusseldorf", "LH74", "Z12" ],
        [ "09:20", "Gothenburg", "LH810", "D53" ],
        [ "09:20", "Faro", "LH1162", "B56" ],
        [ "09:25", "Brussels", "LH1008", "D30" ],
        [ "09:25", "Timisoara", "LH1470", "A5" ],
        [ "09:30", "Bridgetown", "DE3850", "D32" ],
        [ "09:30", "Zagreb", "OU417", "Z14" ],
        [ "09:30", "Tallinn", "LH880", "D26" ],
        [ "09:30", "Copenhagen", "SK640", "A39" ],
        [ "09:35", "Malaga", "LH1148", "B11" ],
        [ "09:35", "Porto", "LH1176", "E20" ],
        [ "09:35", "Algiers", "LH1316", "D31" ],
        [ "09:35", "Kyiv", "LH1490", "A37" ],
        [ "09:40", "Paris", "AF1519", "D15" ],
        [ "09:40", "Cancun", "DE2114", "D24" ],
        [ "09:40", "Dublin", "LH978", "Z56" ],
        [ "09:40", "Valencia", "LH1160", "B57" ],
        [ "09:40", "Dubai", "EK44", "B12" ],
        [ "09:40", "Zurich", "LH1186", "B48" ],
        [ "09:40", "Chisinau", "LH1474", "A54" ],
        [ "09:45", "Toronto", "AC873", "D18" ],
        [ "09:45", "Berlin", "LH178", "A54" ],
        [ "09:45", "Athens", "LH1278", "Z50" ],
        [ "09:50", "Hannover", "LH50", "B15" ],
        [ "09:50", "Stuttgart", "LH128", "Z50" ],
        [ "09:50", "Paderborn", "LH362", "A19" ],
        [ "09:50", "Helsinki", "LH848", "B53" ],
        [ "09:50", "Vienna", "OS122", "E15" ],
        [ "09:50", "New York", "DL107", "B32" ],
        [ "09:50", "Miami", "LH462", "A36" ],
        [ "10:00", "San Diego", "LH466", "B43" ],
        [ "10:00", "London", "LH904", "A40" ],
        [ "10:00", "Bordeaux", "LH1084", "Z57" ],
        [ "10:00", "Larnaca", "LH1292", "A42" ],
        [ "10:00", "Moscow", "LH1444", "Z13" ],
        [ "10:00", "Hamburg", "LH10", "Z19" ],
        [ "10:00", "Dallas", "LH438", "D34" ],
        [ "10:00", "Luxembourg", "LH392", "B6" ],
        [ "10:05", "Montreal", "AC875", "D56" ],
        [ "10:05", "San Francisco", "LH454", "D38" ],
        [ "10:05", "Muscat", "WY116", "A65" ],
        [ "10:10", "Tel Aviv", "LH686", "A10" ],
        [ "10:10", "Oslo", "LH860", "C17" ],
        [ "10:10", "Madrid", "LH1112", "B6" ],
        [ "10:10", "Barcelona", "LH1126", "A8" ],
        [ "10:10", "Krakow", "LH1366", "D18" ],
        [ "10:15", "Charlotte", "AA705", "D57" ],
        [ "10:15", "Amsterdam", "KL1764", "B10" ],
        [ "10:15", "Munich", "LH100", "B2" ],
        [ "10:15", "Nurnberg", "LH142", "A67" ],
        [ "10:15", "Houston", "LH440", "A5" ],
        [ "10:15", "London", "BA8762", "Z41" ],
        [ "10:15", "Vancouver", "LH492", "Z44" ],
        [ "10:15", "Budapest", "LH1336", "Z30" ],
        [ "10:15", "Prague", "LH1394", "C12" ],
        [ "10:20", "Chennai", "LH758", "Z48" ],
        [ "10:25", "Punta Cana", "DE2226", "A59" ],
        [ "10:25", "Bridgetown", "DE3766", "Z27" ],
        [ "10:25", "Los Angeles", "LH456", "Z62" ],
        [ "10:25", "Minsk", "LH1486", "D32" ],
        [ "10:30", "Bilbao", "LH1142", "D11" ],
        [ "10:30", "Beijing", "LH8430", "A18" ],
        [ "10:30", "Las Vegas", "EW1274", "D20" ],
        [ "10:30", "Dubrovnik", "OU419", "A66" ],
        [ "10:35", "Toulouse", "LH1094", "D14" ],
        [ "10:35", "Luqa", "LH1276", "C19" ],
        [ "10:35", "Bucharest", "LH1418", "A9" ],
        [ "10:35", "Sofia", "LH1426", "C20" ],
        [ "10:35", "Warsaw", "LO382", "Z40" ],
        [ "10:35", "Detroit", "DL87", "Z51" ],
        [ "10:35", "Seattle", "LH490", "E6" ],
        [ "10:35", "Doha", "QR70", "E15" ],
        [ "10:40", "Taipei", "CI62", "Z65" ],
        [ "10:40", "Atlanta", "LH444", "D43" ],
        [ "10:45", "Dallas", "AA71", "B52" ],
        [ "10:45", "Abu Dhabi", "EY2", "Z31" ],
        [ "10:45", "Chicago", "LH430", "D18" ],
        [ "10:45", "Riga", "LH890", "Z61" ],
        [ "10:45", "Madrid", "UX1502", "Z52" ],
        [ "10:45", "Bahrain", "GF16", "B25" ],
        [ "10:45", "Berlin", "LH180", "A44" ],
        [ "10:50", "Dublin", "EI651", "A61" ],
        [ "10:50", "Vienna", "OS134", "C9" ],
        [ "10:50", "Berlin", "U25544", "D35" ],
        [ "10:50", "New York", "LH400", "Z29" ],
        [ "10:55", "Rome", "LH232", "Z28" ],
        [ "10:55", "Detroit", "LH442", "A8" ],
        [ "10:55", "Tel Aviv", "LY358", "B48" ],
        [ "10:55", "Copenhagen", "LH826", "Z32" ],
        [ "11:00", "Hamburg", "LH12", "Z32" ],
        [ "11:00", "Boston", "LH422", "B48" ],
        [ "11:00", "Tampa", "LH482", "D18" ],
        [ "11:05", "Valencia", "FR2424", "D18" ],
        [ "11:05", "Tashkent", "HY232", "C13" ],
        [ "11:05", "Edinburgh", "LH962", "B38" ],
        [ "11:05", "Barcelona", "LH1130", "B13" ],
        [ "11:05", "Belgrade", "LH1406", "C13" ],
        [ "11:05", "Lagos", "LH568", "Z60" ],
        [ "11:05", "Denver", "UA181", "D26" ],
        [ "11:10", "Reykjavik", "LH856", "Z12" ],
        [ "11:10", "Manchester", "LH942", "A6" ],
        [ "11:10", "Tirana", "LH1424", "C1" ],
        [ "11:10", "Washington", "UA988", "D43" ],
        [ "11:15", "Thessaloniki", "A3531", "A50" ],
        [ "11:15", "Stockholm", "LH802", "B3" ],
        [ "11:15", "Bucharest", "RO302", "A68" ],
        [ "11:15", "Moscow", "SU2307", "A21" ],
        [ "11:20", "Naples", "LH334", "Z29" ],
        [ "11:20", "Nairobi", "LH590", "B51" ],
        [ "11:20", "Vilnius", "LH886", "Z26" ],
        [ "11:25", "Brussels", "LH1010", "E8" ],
        [ "11:25", "Atlanta", "DL15", "A68" ],
        [ "11:25", "New York", "UA961", "Z55" ],
        [ "11:30", "Hurghada", "DE30", "D22" ],
        [ "11:30", "Funchal", "DE1570", "E18" ],
        [ "11:30", "Venice", "LH326", "B32" ],
        [ "11:30", "Tunis", "TU745", "C3" ],
        [ "11:30", "Tokyo", "NH204", "Z14" ],
        [ "11:35", "Helsinki", "AY1412", "B50" ],
        [ "11:35", "Bari", "LH294", "D26" ],
        [ "11:35", "Gothenburg", "LH816", "A63" ],
        [ "11:40", "London", "BA8733", "B56" ],
        [ "11:40", "Singapore", "SQ25", "Z41" ],
        [ "11:45", "Berlin", "LH182", "D54" ],
        [ "11:45", "Milan", "LH272", "E20" ],
        [ "11:50", "Amsterdam", "KL1766", "A28" ],
        [ "11:50", "Seville", "LH1140", "B53" ],
        [ "11:50", "Marrakesh", "LH1332", "B20" ],
        [ "11:50", "Moscow", "RU418", "Z39" ],
        [ "11:50", "Geneva", "LH1216", "D47" ],
        [ "11:50", "Vienna", "LH1238", "A25" ],
        [ "11:55", "Oslo", "SK4756", "B51" ],
        [ "12:00", "London", "LH906", "B3" ],
        [ "12:00", "Birmingham", "LH954", "Z30" ],
        [ "12:00", "Toulouse", "LH1096", "D34" ],
        [ "12:00", "Palma de Mallorca", "LH1158", "B1" ],
        [ "12:00", "Gdansk", "LH1376", "A25" ],
        [ "12:00", "Paris", "AF1619", "A31" ],
        [ "12:00", "Beirut", "ME218", "Z65" ],
        [ "12:05", "London", "BA907", "A11" ],
        [ "12:05", "Split", "OU413", "C16" ],
        [ "12:10", "Billund", "LH838", "Z56" ],
        [ "12:10", "Wroclaw", "LH1372", "B6" ],
        [ "12:10", "Bydgoszcz", "LH1382", "E4" ],
        [ "12:10", "Madrid", "IB8707", "A65" ],
        [ "12:10", "Istanbul", "TK1588", "D59" ],
        [ "12:15", "Munich", "LH104", "B27" ],
        [ "12:15", "Stockholm", "SK2636", "D29" ],
        [ "12:20", "Copenhagen", "LH828", "A67" ],
        [ "12:20", "Paris", "LH1034", "C7" ],
        [ "12:20", "Budapest", "LH1338", "B8" ],
        [ "12:20", "Florence", "EN8854", "D55" ],
        [ "12:20", "Riyadh", "LH636", "C18" ],
        [ "12:20", "Amsterdam", "LH992", "A10" ],
        [ "12:20", "Warsaw", "LH1348", "Z25" ],
        [ "12:20", "Katowice", "LH1356", "Z66" ],
        [ "12:20", "Graz", "OS254", "A50" ],
        [ "12:25", "Turin", "EN8844", "A33" ],
        [ "12:30", "Dubai", "EK9978", "D17" ],
        [ "12:30", "Dresden", "LH210", "A46" ],
        [ "12:30", "Bologna", "LH284", "A26" ],
        [ "12:30", "Athens", "LH1282", "B23" ],
        [ "12:30", "Tenerife", "X32142", "A56" ],
        [ "12:30", "Gran Canaria", "X32258", "D55" ],
        [ "12:30", "Verona", "EN8826", "B52" ],
        [ "12:30", "Leipzig", "LH158", "A21" ],
        [ "12:30", "Mumbai", "LH756", "D29" ],
        [ "12:30", "Innsbruck", "OS286", "D23" ],
        [ "12:35", "Las Vegas", "DE2062", "D35" ],
        [ "12:40", "Nurnberg", "LH144", "Z43" ],
        [ "12:40", "Milan", "LH252", "Z11" ],
        [ "12:40", "Lyon", "LH1076", "A2" ],
        [ "12:40", "Hong Kong", "CX288", "A13" ],
        [ "12:40", "Luxembourg", "LH394", "B26" ],
        [ "12:40", "Oslo", "LH862", "A68" ],
        [ "12:40", "Basel", "LH1204", "Z26" ],
        [ "12:40", "Moscow", "LH1446", "Z21" ],
        [ "12:40", "Salzburg", "OS264", "A66" ],
        [ "12:45", "Berlin", "LH184", "D29" ],
        [ "12:50", "Stuttgart", "LH130", "Z28" ],
        [ "12:50", "Venice", "LH328", "A41" ],
        [ "12:50", "Bremen", "LH354", "Z66" ],
        [ "12:50", "Bengaluru", "LH754", "A42" ],
        [ "12:50", "Istanbul", "LH1298", "B58" ],
        [ "12:50", "Vienna", "OS130", "A12" ],
        [ "12:50", "Addis Ababa", "LH598", "Z24" ],
        [ "12:50", "San Francisco", "UA59", "A9" ],
        [ "12:55", "Washington", "LH418", "D13" ],
        [ "13:00", "Hamburg", "LH16", "Z61" ],
        [ "13:00", "Dusseldorf", "LH76", "E23" ],
        [ "13:00", "Milan", "LH280", "A19" ],
        [ "13:00", "Cairo", "LH580", "Z55" ],
        [ "13:00", "Madrid", "LH1114", "D46" ],
        [ "13:00", "Philadelphia", "LH426", "Z58" ],
        [ "13:00", "Zagreb", "LH1414", "Z42" ],
        [ "13:00", "Chicago", "UA906", "C13" ],
        [ "13:05", "Reykjavik", "FI521", "A5" ],
        [ "13:05", "Zurich", "LH1190", "B10" ],
        [ "13:05", "Prague", "LH1396", "B57" ],
        [ "13:10", "Milan", "FR1688", "Z65" ],
        [ "13:10", "Munster", "LH64", "D31" ],
        [ "13:10", "Manchester", "LH938", "Z41" ],
        [ "13:10", "Orlando", "LH464", "A41" ],
        [ "13:10", "Geneva", "LX1083", "C18" ],
        [ "13:15", "Lisbon", "LH1168", "D1" ],
        [ "13:15", "Poznan", "LH1390", "B50" ],
        [ "13:15", "Munich", "LH106", "A21" ],
        [ "13:15", "Dubai", "LH630", "A69" ],
        [ "13:15", "Friedrichshafen", "LH372", "D24" ],
        [ "13:15", "Delhi", "LH760", "A63" ],
        [ "13:20", "Hannover", "LH52", "Z27" ],
        [ "13:20", "Stockholm", "LH806", "A61" ],
        [ "13:20", "St. Petersburg", "LH1436", "Z41" ],
        [ "13:20", "Kuwait City", "LH624", "A27" ],
        [ "13:20", "Almaty", "LH648", "A44" ],
        [ "13:25", "Helsinki", "LH850", "B63" ],
        [ "13:25", "Brussels", "LH1012", "B1" ],
        [ "13:25", "Barcelona", "LH1128", "Z24" ],
        [ "13:25", "Bogota", "LH542", "A25" ],
        [ "13:25", "Toronto", "LH470", "B34" ],
        [ "13:30", "Calgary", "AC845", "B13" ],
        [ "13:30", "San Jose", "LH518", "Z43" ],
        [ "13:30", "Denver", "LH446", "E17" ],
        [ "13:30", "Tehran", "LH600", "A64" ],
        [ "13:35", "New York", "LH402", "C16" ],
        [ "13:35", "Mexico City", "LH498", "C20" ],
        [ "13:35", "Tallinn", "LH882", "Z31" ],
        [ "13:35", "Linz", "LH1252", "D24" ],
        [ "13:35", "Lisbon", "TP583", "E22" ],
        [ "13:35", "Shanghai", "LH8400", "Z39" ],
        [ "13:35", "Shanghai", "MU220", "A54" ],
        [ "13:40", "Baku", "LH612", "Z68" ],
        [ "13:40", "Porto", "LH1178", "Z42" ],
        [ "13:45", "Berlin", "LH186", "E20" ],
        [ "13:45", "Moscow", "SU2301", "A21" ],
        [ "13:45", "Antalya", "XQ141", "B6" ],
        [ "13:45", "Bangkok", "TG921", "Z65" ],
        [ "13:45", "Istanbul", "TK1618", "B53" ],
        [ "13:50", "Glasgow", "LH960", "B43" ],
        [ "13:50", "Kyiv", "LH1494", "C7" ],
        [ "13:50", "Houston", "UA47", "Z23" ],
        [ "13:55", "Ho Chi Minh City", "VN30", "D17" ],
        [ "13:55", "Hanoi", "VN36", "Z49" ],
        [ "14:00", "Cancun", "LH514", "D45" ],
        [ "14:00", "London", "LH908", "B3" ],
        [ "14:00", "Hamburg", "LH18", "D16" ],
        [ "14:05", "Tel Aviv", "LH694", "C10" ],
        [ "14:05", "Beijing", "CA966", "Z18" ],
        [ "14:05", "Zurich", "LX1073", "A67" ],
        [ "14:05", "Istanbul", "PC994", "D21" ],
        [ "14:15", "Amsterdam", "KL1768", "E12" ],
        [ "14:15", "Munich", "LH108", "B56" ],
        [ "14:15", "Kyiv", "PS402", "C18" ],
        [ "14:15", "Jeddah", "LH652", "B37" ],
        [ "14:30", "Dubai", "EK46", "Z55" ],
        [ "14:30", "Cluj-Napoca", "LH1472", "E16" ],
        [ "14:35", "Paris", "AF1219", "C1" ],
        [ "14:40", "Milan", "AZ409", "E18" ],
        [ "14:45", "Faro", "FR4160", "B10" ],
        [ "14:45", "Berlin", "LH188", "D37" ],
        [ "14:45", "Amman", "RJ126", "D27" ],
        [ "14:50", "Amman", "LH692", "D54" ],
        [ "14:50", "Valencia", "LH1464", "A36" ],
        [ "14:50", "Vienna", "OS124", "D54" ],
        [ "15:00", "Hamburg", "LH20", "B46" ],
        [ "15:00", "Bilbao", "LH1144", "E6" ],
        [ "15:00", "Sofia", "LH1430", "A53" ],
        [ "15:00", "Izmir", "XQ971", "C13" ],
        [ "15:00", "London", "LH910", "Z22" ],
        [ "15:05", "Istanbul", "TK1592", "B59" ],
        [ "15:05", "London", "BA905", "D46" ],
        [ "15:10", "Palma de Mallorca", "LH1152", "Z67" ],
        [ "15:10", "Sofia", "W64330", "B36" ],
        [ "15:25", "Shanghai", "CZ464", "B53" ],
        [ "15:25", "Arbil", "IA236", "Z15" ],
        [ "15:25", "Cairo", "MS786", "A11" ],
        [ "15:30", "Havana", "DE2184", "D54" ],
        [ "15:30", "Doha", "QR68", "C5" ],
        [ "15:30", "Milan", "LH274", "B41" ],
        [ "15:40", "Guangzhou", "CZ470", "D46" ],
        [ "15:45", "Berlin", "LH190", "E23" ],
        [ "15:50", "Budapest", "LH1344", "B18" ],
        [ "15:50", "Mumbai", "LH8360", "A23" ],
        [ "15:50", "Alicante", "FR4120", "D54" ],
        [ "15:50", "Vienna", "LH1240", "A57" ],
        [ "16:00", "Stockholm", "LH804", "A4" ],
        [ "16:00", "Dublin", "LH980", "C1" ],
        [ "16:00", "Barcelona", "LH1132", "B15" ],
        [ "16:00", "Hamburg", "LH22", "B9" ],
        [ "16:00", "Oslo", "LH858", "E11" ],
        [ "16:00", "London", "LH914", "A40" ],
        [ "16:00", "Copenhagen", "SK676", "Z42" ],
        [ "16:05", "Rome", "LH238", "B25" ],
        [ "16:10", "London", "LH934", "B20" ],
        [ "16:10", "Geneva", "LH1222", "D21" ],
        [ "16:10", "Tel Aviv", "LH8340", "Z34" ],
        [ "16:15", "Munich", "LH112", "B46" ],
        [ "16:15", "Riyadh", "SV180", "Z12" ],
        [ "16:15", "Berlin", "LH166", "D5" ],
        [ "16:15", "Krakow", "LH1368", "D58" ],
        [ "16:25", "Gothenburg", "LH818", "D47" ],
        [ "16:25", "Manchester", "LH946", "E23" ],
        [ "16:25", "Edinburgh", "LH964", "A4" ],
        [ "16:25", "Brussels", "LH1016", "Z43" ],
        [ "16:25", "Zurich", "LH1196", "D20" ],
        [ "16:30", "Amsterdam", "LH996", "B17" ],
        [ "16:30", "Paris", "LH1040", "D6" ],
        [ "16:30", "Moscow", "LH1452", "D34" ],
        [ "16:40", "Milan", "LH254", "C13" ],
        [ "16:40", "Lyon", "LH1078", "D56" ],
        [ "16:40", "Marseille", "LH1088", "B7" ],
        [ "16:40", "Toulouse", "LH1098", "A60" ],
        [ "16:40", "Katowice", "LH1360", "B14" ],
        [ "16:40", "Florence", "EN8858", "D29" ],
        [ "16:40", "Warsaw", "LH1350", "E9" ],
        [ "16:40", "Ljubljana", "LH1462", "B20" ],
        [ "16:40", "Salzburg", "OS266", "D19" ],
        [ "16:45", "Barcelona", "FR1680", "D7" ],
        [ "16:45", "Berlin", "LH192", "B54" ],
        [ "16:45", "Budapest", "LH1340", "B23" ],
        [ "16:50", "Chicago", "LH432", "A37" ],
        [ "16:50", "Nice", "LH1064", "E13" ],
        [ "16:50", "Vienna", "LH1242", "A52" ],
        [ "16:50", "Verona", "EN8828", "A1" ],
        [ "16:50", "Friedrichshafen", "LH374", "C2" ],
        [ "16:50", "Madrid", "LH1122", "D20" ],
        [ "16:50", "Graz", "OS256", "D6" ],
        [ "16:50", "Innsbruck", "OS284", "A47" ],
        [ "16:55", "Birmingham", "LH956", "Z27" ],
        [ "16:55", "Istanbul", "LH1304", "Z18" ],
        [ "16:55", "Turin", "EN8848", "D49" ],
        [ "16:55", "Billund", "LH840", "B55" ],
        [ "16:55", "Barcelona", "LH1134", "Z69" ],
        [ "17:00", "Dusseldorf", "LH84", "D10" ],
        [ "17:00", "Bologna", "LH288", "A47" ],
        [ "17:00", "Venice", "LH330", "B43" ],
        [ "17:00", "Prague", "LH1400", "E2" ],
        [ "17:00", "Hamburg", "LH24", "Z38" ],
        [ "17:00", "Beijing", "LH720", "D5" ],
        [ "17:00", "London", "LH916", "A11" ],
        [ "17:00", "Linz", "LH1254", "A29" ],
        [ "17:10", "Toronto", "AC877", "A36" ],
        [ "17:10", "Leipzig", "LH160", "D34" ],
        [ "17:10", "Dresden", "LH214", "B2" ],
        [ "17:10", "Milan", "LH276", "A43" ],
        [ "17:10", "Copenhagen", "LH830", "Z54" ],
        [ "17:10", "Wroclaw", "LH1354", "D37" ],
        [ "17:10", "Kyiv", "LH1492", "B18" ],
        [ "17:10", "Shanghai", "CK212", "E20" ],
        [ "17:10", "London", "LH936", "B37" ],
        [ "17:10", "Washington", "UA933", "B61" ],
        [ "17:15", "Munich", "LH114", "Z32" ],
        [ "17:15", "Berlin", "LH168", "A19" ],
        [ "17:15", "Budapest", "W62312", "E4" ],
        [ "17:20", "Hannover", "LH54", "A5" ],
        [ "17:20", "Stuttgart", "LH132", "B47" ],
        [ "17:20", "Nurnberg", "LH148", "B35" ],
        [ "17:20", "Luxembourg", "LH396", "C17" ],
        [ "17:20", "Geneva", "LX1085", "A35" ],
        [ "17:25", "Brussels", "LH1018", "A54" ],
        [ "17:25", "New York", "LH404", "A31" ],
        [ "17:25", "Bremen", "LH356", "D31" ],
        [ "17:25", "San Francisco", "UA927", "B8" ],
        [ "17:30", "Paris", "LH1046", "A17" ],
        [ "17:30", "Stockholm", "SK638", "A62" ],
        [ "17:30", "Nanjing", "LH780", "C16" ],
        [ "17:30", "Basel", "LH1206", "C10" ],
        [ "17:30", "Minsk", "LH1488", "A69" ],
        [ "17:30", "Zurich", "LX1075", "D7" ],
        [ "17:35", "Munster", "LH66", "D24" ],
        [ "17:35", "Seoul", "LH712", "Z26" ],
        [ "17:35", "Seoul", "KE536", "B60" ],
        [ "17:35", "Seoul", "OZ542", "B27" ],
        [ "17:40", "Casablanca", "AT811", "C12" ],
        [ "17:40", "London", "BA909", "D10" ],
        [ "17:40", "Milan", "LH256", "B43" ],
        [ "17:40", "Amsterdam", "LH998", "D4" ],
        [ "17:40", "Moscow", "SU2657", "E26" ],
        [ "17:45", "Berlin", "LH194", "D3" ],
        [ "17:45", "Paris", "AF1319", "A48" ],
        [ "17:50", "London", "BA8735", "D48" ],
        [ "17:50", "Amsterdam", "KL1770", "Z40" ],
        [ "17:50", "Vienna", "OS132", "B63" ],
        [ "17:55", "Tel Aviv", "LH690", "D58" ],
        [ "18:00", "Athens", "A3833", "Z68" ],
        [ "18:00", "Shenyang", "LH782", "D57" ],
        [ "18:00", "London", "LH918", "D35" ],
        [ "18:00", "Zagreb", "OU411", "B58" ],
        [ "18:00", "London", "FR1546", "D18" ],
        [ "18:00", "Hamburg", "LH28", "E17" ],
        [ "18:00", "Tokyo", "LH716", "B29" ],
        [ "18:05", "Shanghai", "LH728", "B24" ],
        [ "18:05", "Istanbul", "TK1594", "B8" ],
        [ "18:10", "St. Petersburg", "LH1438", "E3" ],
        [ "18:15", "Rome", "AZ407", "B22" ],
        [ "18:15", "Mexico City", "EK9933", "B11" ],
        [ "18:15", "Munich", "LH116", "A40" ],
        [ "18:15", "Berlin", "LH196", "E9" ],
        [ "18:20", "Zurich", "LH1198", "E20" ],
        [ "18:25", "Chicago", "LH8186", "B26" ],
        [ "18:30", "Atlanta", "LH8174", "Z16" ],
        [ "18:40", "Berlin", "U25552", "A18" ],
        [ "18:45", "Berlin", "LH198", "D36" ],
        [ "18:45", "Moscow", "P3412", "B39" ],
        [ "18:45", "London", "BA911", "D57" ],
        [ "18:50", "Riga", "BT246", "C7" ],
        [ "18:55", "Nur-Sultan", "KC922", "A45" ],
        [ "19:00", "Shanghai", "CA936", "E16" ],
        [ "19:00", "Hamburg", "LH30", "Z47" ],
        [ "19:05", "Madrid", "IB8711", "Z14" ],
        [ "19:10", "Windhoek", "SW286", "Z21" ],
        [ "19:10", "Madrid", "UX1506", "Z49" ],
        [ "19:15", "Munich", "LH118", "Z56" ],
        [ "19:15", "Sofia", "LH1428", "A22" ],
        [ "19:15", "Beijing", "CA932", "A27" ],
        [ "19:20", "Amsterdam", "KL1774", "D38" ],
        [ "19:20", "Lisbon", "TP579", "A68" ],
        [ "19:25", "Helsinki", "AY1416", "Z19" ],
        [ "19:25", "Madrid", "LA705", "B33" ],
        [ "19:25", "Bucharest", "RO306", "D40" ],
        [ "19:30", "Seoul", "KE906", "B43" ],
        [ "19:30", "Tel Aviv", "LY356", "D49" ],
        [ "19:30", "Tianjin", "CA8404", "B2" ],
        [ "19:35", "Bengaluru", "LH8474", "D35" ],
        [ "19:40", "Milan", "AZ419", "C1" ],
        [ "19:40", "Tokyo", "JL408", "D35" ],
        [ "19:40", "Riyadh", "SV930", "A33" ],
        [ "19:45", "Seoul", "KE538", "A57" ],
        [ "19:45", "Berlin", "LH200", "B7" ],
        [ "19:50", "Cairo", "LH8294", "D52" ],
        [ "19:50", "Warsaw", "LO380", "Z11" ],
        [ "19:50", "Prague", "OK537", "Z53" ],
        [ "19:50", "Vienna", "OS126", "B16" ],
        [ "19:50", "Stockholm", "SK674", "D49" ],
        [ "19:50", "Paris", "AF1119", "A5" ],
        [ "19:50", "Dublin", "EI657", "C11" ],
        [ "20:00", "Hamburg", "LH32", "D13" ],
        [ "20:00", "London", "LH920", "D37" ],
        [ "20:00", "Moscow", "RU466", "Z43" ],
        [ "20:00", "Windhoek", "EW1278", "B41" ],
        [ "20:10", "London", "BA8737", "E17" ],
        [ "20:10", "Beijing", "CA1032", "Z64" ],
        [ "20:15", "Munich", "LH120", "B31" ],
        [ "20:15", "Beirut", "LH1306", "B30" ],
        [ "20:15", "Berlin", "LH44", "C3" ],
        [ "20:20", "Sao Paulo", "LA8071", "B40" ],
        [ "20:20", "Tallinn", "LH884", "B62" ],
        [ "20:20", "Porto", "LH1180", "A47" ],
        [ "20:30", "Dubai", "EK48", "D37" ],
        [ "20:30", "Dublin", "FR382", "D36" ],
        [ "20:30", "Hamburg", "LH36", "B7" ],
        [ "20:30", "Marseille", "LH1090", "Z65" ],
        [ "20:30", "Copenhagen", "SK1636", "A63" ],
        [ "20:30", "Dubai", "X36888", "D22" ],
        [ "20:30", "Madrid", "BA9042", "C12" ],
        [ "20:30", "Madrid", "IB9042", "D30" ],
        [ "20:30", "Milan", "LH258", "Z33" ],
        [ "20:30", "Bucharest", "LH1422", "Z36" ],
        [ "20:35", "Berlin", "U25554", "D47" ],
        [ "20:40", "Bologna", "LH290", "E21" ],
        [ "20:40", "Vilnius", "LH888", "Z21" ],
        [ "20:40", "Lisbon", "LH1172", "Z40" ],
        [ "20:40", "Zagreb", "LH1404", "A56" ],
        [ "20:40", "New York", "LH8160", "A63" ],
        [ "20:40", "Seoul", "OZ794", "A59" ],
        [ "20:40", "Paris", "LH1050", "B18" ],
        [ "20:45", "Tirana", "LH1454", "B56" ],
        [ "20:45", "Tokyo", "NH224", "A62" ],
        [ "20:45", "Johannesburg", "SA261", "Z64" ],
        [ "20:50", "Amsterdam", "LH1002", "E22" ],
        [ "20:50", "Toulouse", "LH1100", "B21" ],
        [ "20:50", "Athens", "LH1284", "A40" ],
        [ "20:50", "Mexico City", "LH8058", "E16" ],
        [ "20:50", "Friedrichshafen", "LH376", "D48" ],
        [ "20:50", "Madrid", "LH1120", "A67" ],
        [ "20:50", "Zurich", "LX1077", "Z35" ],
        [ "20:55", "Zagreb", "OU415", "E3" ],
        [ "20:55", "Salzburg", "OS270", "D19" ],
        [ "20:55", "Bangkok", "TG923", "B12" ],
        [ "21:00", "Rome", "LH242", "A25" ],
        [ "21:00", "Bilbao", "LH1146", "E20" ],
        [ "21:00", "Valencia", "LH1164", "C5" ],
        [ "21:00", "Abu Dhabi", "EY8", "D47" ],
        [ "21:00", "Munster", "LH68", "D49" ],
        [ "21:00", "Basel", "LH1208", "D5" ],
        [ "21:00", "Prague", "LH1402", "E16" ],
        [ "21:05", "Geneva", "LX1087", "Z64" ],
        [ "21:10", "Warsaw", "LH1352", "Z69" ],
        [ "21:10", "Poznan", "LH1380", "D18" ],
        [ "21:10", "Tokyo", "LH8386", "B58" ],
        [ "21:15", "Berlin", "LH202", "B10" ],
        [ "21:15", "Barcelona", "LH1138", "D46" ],
        [ "21:15", "Doha", "QR8224", "Z39" ],
        [ "21:15", "Munich", "LH122", "C2" ],
        [ "21:20", "Hannover", "LH58", "Z14" ],
        [ "21:20", "Nurnberg", "LH150", "D48" ],
        [ "21:20", "Dresden", "LH216", "Z20" ],
        [ "21:20", "Bremen", "LH360", "D5" ],
        [ "21:20", "Nice", "LH1068", "B50" ],
        [ "21:20", "Budapest", "LH1342", "E23" ],
        [ "21:20", "Verona", "EN8830", "D34" ],
        [ "21:25", "Riga", "LH892", "B60" ],
        [ "21:25", "Brussels", "LH1024", "D21" ],
        [ "21:30", "Dusseldorf", "LH88", "Z38" ],
        [ "21:30", "Venice", "LH332", "B33" ],
        [ "21:30", "Luxembourg", "LH398", "D55" ],
        [ "21:30", "Stockholm", "LH808", "A50" ],
        [ "21:30", "London", "LH922", "Z61" ],
        [ "21:30", "Delhi", "AI120", "D21" ],
        [ "21:30", "Turin", "EN8850", "E15" ],
        [ "21:30", "Florence", "EN8860", "D59" ],
        [ "21:30", "Hamburg", "LH34", "B6" ],
        [ "21:35", "Addis Ababa", "ET707", "D24" ],
        [ "21:35", "Helsinki", "LH854", "A53" ],
        [ "21:40", "Oslo", "LH864", "C12" ],
        [ "21:40", "Vienna", "LH1246", "B7" ],
        [ "21:40", "Krakow", "LH1370", "A47" ],
        [ "21:40", "Graz", "OS258", "A17" ],
        [ "21:40", "Gothenburg", "LH820", "B59" ],
        [ "21:45", "Innsbruck", "OS288", "B20" ],
        [ "21:50", "Leipzig", "LH164", "A61" ],
        [ "21:50", "Milan", "LH278", "B24" ],
        [ "21:50", "Manchester", "LH948", "B25" ],
        [ "21:50", "Lyon", "LH1082", "A44" ],
        [ "21:50", "Belgrade", "LH1410", "B9" ],
        [ "21:50", "Luanda", "LH560", "Z39" ],
        [ "21:50", "Dublin", "LH982", "E24" ],
        [ "21:50", "Gdansk", "LH1378", "B12" ],
        [ "21:55", "Cape Town", "DE2290", "A26" ],
        [ "21:55", "Birmingham", "LH958", "C5" ],
        [ "21:55", "Linz", "LH1256", "A35" ],
        [ "21:55", "Rio de Janeiro", "LH500", "B62" ],
        [ "21:55", "Singapore", "LH778", "A12" ],
        [ "21:55", "Billund", "LH842", "Z69" ],
        [ "21:55", "Wroclaw", "LH1374", "A3" ],
        [ "21:55", "Singapore", "SQ325", "D43" ],
        [ "22:00", "Copenhagen", "LH832", "A16" ],
        [ "22:00", "Johannesburg", "LH572", "E3" ],
        [ "22:00", "Bangkok", "LH772", "C18" ],
        [ "22:05", "Sao Paulo", "LH506", "Z69" ],
        [ "22:05", "Tunis", "LH1326", "B57" ],
        [ "22:05", "Katowice", "LH1362", "D10" ],
        [ "22:15", "Stuttgart", "LH136", "E3" ],
        [ "22:15", "Hong Kong", "LH796", "Z34" ],
        [ "22:15", "Moscow", "SU2305", "D32" ],
        [ "22:15", "Buenos Aires", "LH510", "D50" ]
    ]
}
