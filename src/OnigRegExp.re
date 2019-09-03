
type t;

external _create: string => result(t, string) = "reonig_create";

let create = (re: string) => {
    _create(re);
};
