
type t;


type rawMatch = (int, int);

module Match = {
    type t = {
        index: int,
        startPos: int,
        length: int,
        endPos: int,
        match: string,
    }

    let ofRaw = (~index, ~rawMatch, str) => {
        let (startPos, length) = rawMatch;

        let match = String.sub(str, startPos, length);

        {
            index,
            startPos,
            endPos: startPos + length,
            length,
            match,
        };
    };
};

external _create: string => result(t, string) = "reonig_create";
external _search: (string, int, t) => array(rawMatch) = "reonig_search";
external _end: unit => unit = "reonig_end";

let create = (re: string) => {
    _create(re);
};

let search = (str: string, startPosition: int, regexp: t) => {
    let matches = _search(str, startPosition, regexp);
    Array.mapi((idx, m) => {
        Match.ofRaw(~index=idx, ~rawMatch=m, str);
    }, matches);
};

at_exit(_end);
