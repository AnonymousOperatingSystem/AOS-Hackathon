
package plus.aos.jaos.core.common;

import java.util.List;

import lombok.Data;

@Data
public class Action {
    private String account;
    private List<Authorization> authorization;
    private Object data;
    private String name;
    private String hexData;

}
