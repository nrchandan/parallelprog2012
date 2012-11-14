package benchmarks;

import java.lang.reflect.Field;
import java.util.Map;
import java.util.HashMap;
public class Environment {
  @SuppressWarnings("unchecked")
  public static Map<String, String> getenv() {
    try {
      Map<String, String> unomdifiable = System.getenv();
      Class<?> cu = unomdifiable.getClass();
      Field m = cu.getDeclaredField("m");
      m.setAccessible(true);
      return (Map<String, String>)m.get(unomdifiable);
    }
    catch (Exception e) { }
    return new HashMap<String, String>();
  }
}
