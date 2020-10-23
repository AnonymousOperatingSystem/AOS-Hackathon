package plus.aos.jaos.core.response.chain.account;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class Limit {
    private Long available;
    private Long max;
    private Long used;
}
