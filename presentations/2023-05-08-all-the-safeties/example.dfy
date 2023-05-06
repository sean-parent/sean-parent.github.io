
method Reverse(a: seq<int>) returns (r: seq<int>)
  ensures |a| == |r|
  ensures forall i :: 0 <= i < |a| ==> r[i] == a[|a| - i - 1]
{
  var remainder := a;
  r := [];
  while (|remainder| > 1)
    invariant |remainder| == |a| - |r|
    invariant forall i :: 0 <= i < |r| ==> r[i] == a[|r| - i - 1]
    invariant forall i :: 0 <= i < |remainder| ==> remainder[i] == a[|r| + i]
  {
    r := [remainder[0]] + r;
    remainder := remainder[1..];
  }
  r := remainder + r;
}

method Main() {
  var a := [1, 2, 3];
  var b := Reverse(a);
  assert b == [3, 2, 1];
}
