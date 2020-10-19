package plus.aos.jaos;

import plus.aos.jaos.impl.AosApiRestClientImpl;
import plus.aos.jaos.impl.LocalApiImpl;

public abstract class AosApiFactory {
    /**
     * 从节点获得api接口
     * create api from endpoint
     * <p>server list: https://www.aos.plus/</p>
     * @param baseUrl 节点地址 the endpoint address
     * @return api client
     */
    public static AosApi create(String baseUrl) {
        return new AosApiRestClientImpl(baseUrl);
    }

    /**
     * 从节点获得api接口
     * create api from endpoint
     * <p>server list: https://www.aos.plus/</p>
     * @param baseUrl   节点地址 the endpoint address
     * @param bearerToken 验证信息 bearerToken
     * @return api client
     */
    public static AosApi create(String baseUrl, String bearerToken) {
        return new AosApiRestClientImpl(baseUrl, bearerToken);
    }

    public static AosApi create(String walletBaseUrl, String chainBaseUrl, String historyBaseUrl) {
        return new AosApiRestClientImpl(walletBaseUrl, chainBaseUrl, historyBaseUrl);
    }

    public static AosApi create(String walletBaseUrl, String chainBaseUrl, String historyBaseUrl, String bearerToken) {
        return new AosApiRestClientImpl(walletBaseUrl, chainBaseUrl, historyBaseUrl, bearerToken);
    }
    
    public static LocalApi createLocalApi() {
        return new LocalApiImpl();
    }
}
