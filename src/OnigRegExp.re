
type t;

external _create: string => result(t, string) = "reonig_create";

let create = (re: string) => {
    _create(re);
};

module Match = {
    type t = {
        index: int,
        startPos: int,
        endPos: int, 
        match: string,
        length: int,
    }
};

let search = (_str: string, _startPosition: int, _regexp: t) => {
    None;
};
