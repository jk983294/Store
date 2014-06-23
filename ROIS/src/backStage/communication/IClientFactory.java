package backStage.communication;
/**
 * 通讯模块工厂
 * @author zhd
 * @date 2011-4-19
 */
public class IClientFactory {
	public static ROIS_Client getClient(String server_ip) {
		return new ROIS_Client(server_ip);
	}
}
