package plus.aos.jaos.core.response.chain.account;

import lombok.Data;

@Data
public class Permission {

    private String parent;
    private String permName;
    private RequiredAuth requiredAuth;
}
