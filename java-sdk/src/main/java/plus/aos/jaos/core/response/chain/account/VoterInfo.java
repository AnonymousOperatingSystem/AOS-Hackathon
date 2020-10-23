package plus.aos.jaos.core.response.chain.account;

import java.util.List;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class VoterInfo {
    private Integer isProxy;
    private String lastVoteWeight;
    private String owner;
    private List producers;
    private String proxiedVoteWeight;
    private String proxy;
    private Long staked;
}
