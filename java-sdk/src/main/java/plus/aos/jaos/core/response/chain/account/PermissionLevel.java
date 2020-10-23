package plus.aos.jaos.core.response.chain.account;

import lombok.AllArgsConstructor;
import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
@AllArgsConstructor
public class PermissionLevel {
    private String actor;
    private String permission;
}
