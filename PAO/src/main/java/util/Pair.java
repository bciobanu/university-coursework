package util;

import java.util.Objects;

public class Pair<F, S> implements java.io.Serializable {
  public final F fst;
  public final S snd;

  public Pair(F first, S second) {
    this.fst = first;
    this.snd = second;
  }

  public static <A, B> Pair<A, B> of(A a, B b) {
    return new Pair<A, B>(a, b);
  }

  @Override
  public boolean equals(Object o) {
    if (!(o instanceof Pair)) {
      return false;
    }
    Pair<?, ?> p = (Pair<?, ?>) o;
    return Objects.equals(p.fst, fst) && Objects.equals(p.snd, snd);
  }

  @Override
  public int hashCode() {
    return (fst == null ? 0 : fst.hashCode()) ^ (snd == null ? 0 : snd.hashCode());
  }
}