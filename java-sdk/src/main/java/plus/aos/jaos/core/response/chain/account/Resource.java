package plus.aos.jaos.core.response.chain.account;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class Resource {
    private String cpuWeight;
    private String netWeight;
    private String owner;
    private Long ramBytes;
}
