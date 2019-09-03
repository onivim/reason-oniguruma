open TestFramework;

open Oniguruma;

describe("OnigRegExp", ({describe, _}) => {
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
});
