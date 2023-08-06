#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/inet.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ouriel, Alexander");
MODULE_DESCRIPTION("task 6");

// a ip address that will be blocked
static char * block_ip = "";
module_param(block_ip, charp, S_IRUGO);
MODULE_PARM_DESC(block_ip, "ip address to block");

static unsigned int hook_entering_fun(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *ip_header;
    ip_header = ip_hdr(skb);// get the IP header

    if (!skb)
		return NF_ACCEPT;
    // if the ip address is the same as the one we want to block drop the packet

    if (ip_header->saddr == in_aton(block_ip))
    {
        printk(KERN_INFO "Dropping packet from %pI4\n", &ip_header->saddr);
        return NF_DROP; // Drop the packet
    }


    return NF_ACCEPT; // Allow the packet to continue its normal path
}

static struct nf_hook_ops packet_monitor_entring_hook = {
    .hook = hook_entering_fun,
    .hooknum = NF_INET_PRE_ROUTING, // Intercept packets just before they are routed (hook type 1)
    .pf = PF_INET, // IPv4 packets
    .priority = NF_IP_PRI_FIRST, // Highest priority
};

static int __init packet_monitor_init(void) {
	nf_register_net_hook(&init_net, &packet_monitor_entring_hook);
  
    printk(KERN_INFO "Packet Monitor module loaded\n");
    printk(KERN_INFO "Blocking ip address: %s\n", block_ip);
    return 0;
}

static void __exit packet_monitor_exit(void) {
	nf_unregister_net_hook(&init_net, &packet_monitor_entring_hook);
    printk(KERN_INFO "Packet Monitor module unloaded\n");
}

module_init(packet_monitor_init);
module_exit(packet_monitor_exit);





