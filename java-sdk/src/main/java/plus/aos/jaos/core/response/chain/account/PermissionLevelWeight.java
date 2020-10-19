package plus.aos.jaos.core.response.chain.account;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class PermissionLevelWeight {
    private PermissionLevel permission;
    private Integer weight;
}
