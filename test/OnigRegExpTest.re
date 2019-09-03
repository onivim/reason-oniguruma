open TestFramework;

open Oniguruma;

describe("OnigRegExp", ({describe, _}) => {
  describe("allocation", ({test, _}) => {
    test("finalizer gets called for regexp", ({expect, _}) => {
      let regexp = OnigRegExp.create("\\w(\\d+)");
      let callCount = ref(0);
      Gc.finalise_last(() => incr(callCount), regexp);
      Gc.full_major();

      expect.int(callCount^).toBe(1);
    });
  });
  describe("create", ({test, _}) => {
    test("erronous regexp - never ending recursion", ({expect, _}) => {
      let r = OnigRegExp.create("?<abc>\\g<abc>");
      switch (r) {
      | Ok(_) => expect.int(0).toBe(1);
      | Error(msg) => expect.string(msg).toEqual("target of repeat operator is not specified");
      }
      
    });
    test("erronous regexp - undefined callout", ({expect, _}) => {
      let r = OnigRegExp.create("(*FOO)");
      switch (r) {
      | Ok(_) => expect.int(0).toBe(1);
      | Error(msg) => expect.string(msg).toEqual("target of repeat operator is not specified");
      }
      
    });
  });
  describe("search", ({test, _}) => {

    test("returns None if it does not match", ({ expect, _}) => {
      let r = OnigRegExp.create("\\w(\\d+)");
      switch (r) {
      | Error(_) => expect.string("Fail").toEqual("");
      | Ok(regex) => {
        let result = OnigRegExp.search("-----------", 0, regex);
        expect.bool(result == None).toBe(true);
      }
      }
    });
  });
});
